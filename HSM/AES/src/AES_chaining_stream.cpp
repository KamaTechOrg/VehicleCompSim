#include <cstddef>
#include <cstdint>
#include <algorithm>
// #include <execution> 

// for htonl
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

#include "AES_chaining_stream.hpp"
#include "AES_block_encrypt.hpp"



#if SYCL_ENABLED
#include <sycl/sycl.hpp>
#endif

namespace aes {

using AES_State = AES_block_encrypt::State;

/* /////////// ECB mode \\\\\\\\\\\  */

std::vector<uint8_t> AES_ChainingStream::encrypt_ecb(AES_block_encrypt const& aes, std::vector<uint8_t> const& message){
    std::vector<uint8_t> encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN); // encrypted_message.size() == N*sizeof(State)
  
  encrypted_message.insert(encrypted_message.end(), message.begin(), message.end()); // fill message
  encrypted_message.resize(message.size()+paddingN, paddingN); // fill padding char
  auto* states_buf = reinterpret_cast<AES_State*>(encrypted_message.data());
  std::for_each_n(/* std::execution::par,  */states_buf, encrypted_message.size()/16, [&](auto& state){
    aes.encrypt(state);
  });
  return encrypted_message;
}


std::vector<uint8_t> AES_ChainingStream::decrypt_ecb(AES_block_encrypt const& aes, std::vector<uint8_t> const& encrypted_message) {
  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  std::vector<uint8_t> message = encrypted_message;
  auto states_buf = reinterpret_cast<AES_State*>(message.data());
  std::for_each_n(/* std::execution::par, */ states_buf, message.size()/16, [&](auto& state){
    aes.decrypt(state);
  });

  if(message.back() > 16 
    || !std::all_of(
     message.end() - message.back(),
      message.end(),
      [&](uint8_t c){ return c == message.back();})
  ){
    throw std::invalid_argument("invalid padding for AES ECB.");
  }
  message.resize(message.size() - message.back());
  return message;
}

#if SYCL_ENABLED

std::vector<uint8_t> AES_ChainingStream::encrypt_ecb(sycl::queue& q, AES_block_encrypt const& aes, std::vector<uint8_t> const& message)  {
  std::vector<uint8_t> encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN); // encrypted_message.size() == N*sizeof(State)
  
  encrypted_message.insert(encrypted_message.end(), message.begin(), message.end()); // fill message
  encrypted_message.resize(message.size()+paddingN, paddingN); // fill padding

  size_t blocksN = encrypted_message.size()/16;
  sycl::range<1> blocksN_range{blocksN};

  sycl::buffer states_buf(reinterpret_cast<AES_State*>(encrypted_message.data()), blocksN_range);
  sycl::buffer aes_buf(&aes, sycl::range<1>{1});
  q.submit([&](sycl::handler &h){
    sycl::accessor aes_accessor(aes_buf, h, sycl::read_only);
    sycl::accessor states_accessor(states_buf, h, sycl::read_write);
    h.parallel_for(blocksN_range, [=](auto i) {
       aes_accessor[0].encrypt(states_accessor[i]);
    });
  });
  q.wait();
  return encrypted_message;
}


std::vector<uint8_t> AES_ChainingStream::decrypt_ecb(sycl::queue& q, AES_block_encrypt const& aes, std::vector<uint8_t> const& encrypted_message) {
  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  std::vector<uint8_t> message = encrypted_message;

  { 
    sycl::range<1> blocksN_range{message.size()/16};
    sycl::buffer states_buf(reinterpret_cast<AES_State*>(message.data()), blocksN_range);
    sycl::buffer aes_buf(&aes, sycl::range<1>{1});
    q.submit([&](sycl::handler &h){
      sycl::accessor aes_accessor(aes_buf, h, sycl::read_only);
      sycl::accessor states_accessor(states_buf, h, sycl::read_write);
      h.parallel_for(blocksN_range, [=](auto i) {
        aes_accessor[0].decrypt(states_accessor[i]);
      });
    });
    q.wait();
  } // On exiting this block the data should be copied back to the host
  if(message.back() > 16){
    throw std::invalid_argument("aes invalid padding char");
  }
  message.resize(message.size() - message.back());
  return message;
}

#endif


/* /////////// CBC mode \\\\\\\\\\\  */

std::vector<uint8_t> AES_ChainingStream::encrypt_cbc(AES_block_encrypt const& aes, std::array<uint8_t, 16> const& iv, std::vector<uint8_t> const& message) {
  auto Nb = AES_block_encrypt::Nb;

  AES_State prev_state;
  for (uint col = 0; col < Nb; ++col) {
    for (uint row = 0; row < Nb ; ++row) {
      prev_state[col][row] = iv[row + col*4];
    }
  }
  AES_State state;
  std::vector<uint8_t> encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN);
  size_t msg_idx = 0;
  while(msg_idx < message.size()+paddingN){
    for (uint col = 0; col < Nb; ++col) {
      for (uint row = 0; row < Nb ; ++row) {
        if(msg_idx + row + col*4 < message.size()){
          state[col][row] = message[msg_idx + row + col*4]^prev_state[col][row];
        } else {
          uint8_t p1 =  prev_state[col][row];
          uint8_t pad1 = paddingN;
          uint8_t r1 = p1^pad1;
          state[col][row] = r1;
        }
      }
    }
    aes.encrypt(state);
    for (uint col = 0; col < Nb; ++col) {
      for (uint row = 0; row < Nb; ++row) {
        encrypted_message.push_back(state[col][row]);
      }
    }
    prev_state = state;
    msg_idx += Nb*Nb;
  }
  return encrypted_message;
}


std::vector<uint8_t> AES_ChainingStream::decrypt_cbc(AES_block_encrypt const& aes, std::array<uint8_t, 16> const& iv, std::vector<uint8_t> const& encrypted_message)  {
  auto Nb = AES_block_encrypt::Nb;

  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  AES_State prev_state;
  for (uint col = 0; col < Nb; ++col) {
    for (uint row = 0; row < Nb ; ++row) {
      prev_state[col][row] = iv[row + col*4];
    }
  }
  AES_State state;
  std::vector<uint8_t> message;
  message.reserve(encrypted_message.size());
  size_t msg_idx = 0;
  while(msg_idx < encrypted_message.size()){
    for (uint col = 0; col < Nb && msg_idx < encrypted_message.size(); ++col) {
      for (uint row = 0; row < Nb ; ++row) {
          state[col][row] = encrypted_message[msg_idx + row + col*4];
      }
    }
    auto tmp = state;
    aes.decrypt(state);
    for (uint col = 0; col < Nb && msg_idx < encrypted_message.size(); ++col) {
      for (uint row = 0; row < Nb; ++row) {
        message.push_back(state[col][row]^prev_state[col][row]);
      }
    }
    prev_state = tmp;
    msg_idx += Nb*Nb;
  }
  if(message.back() > 16){
    throw std::invalid_argument(std::string("aes invalid padding char."));
  }
  message.resize(message.size() - message.back());
  return message;
}


/* /////////// CTR mode \\\\\\\\\\\  */

struct CtrBlock{
  union {
    AES_State state;
    struct {
      std::array<uint8_t, 4> nonce;
      std::array<uint8_t, 8> iv;
      uint32_t counter;
    };
    std::array<uint8_t, 16> data;
  };
};

std::vector<uint8_t> AES_ChainingStream::encrypt_ctr(AES_block_encrypt const& aes, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce,  std::vector<uint8_t> const& message){
    size_t blocksN = (message.size() + 15) / 16; 
    std::vector<uint8_t> encrypted_message = message;
    size_t msg_idx = 0;
    for(uint32_t counter = 1; counter<=blocksN; ++counter){
      CtrBlock block = {
        .nonce = nonce,
        .iv = iv,
        .counter = htonl(counter)
      };
      aes.encrypt(block.state);
      for(uint8_t c: block.data){
        if(msg_idx >= encrypted_message.size()){
          break;
        }
        encrypted_message[msg_idx++] ^= c;
      }
    }
    return encrypted_message;
}




#if SYCL_ENABLED
std::vector<uint8_t> AES_ChainingStream::encrypt_ctr(sycl::queue& q, AES_block_encrypt const& aes, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce, std::vector<uint8_t> const& message){
    size_t blocksN = message.size() / 16; //entire blocks
    size_t remain = message.size() % 16;
    std::vector<uint8_t> encrypted_message = message;
    if(blocksN) {
      sycl::buffer aes_buf(&aes, sycl::range<1>{1});
      sycl::range<1> blocksN_range{blocksN};
      sycl::buffer msg_buf(encrypted_message.data(), sycl::range<1>{encrypted_message.size()-remain});
      q.submit([&](sycl::handler &h){
        sycl::accessor aes_accessor(aes_buf, h, sycl::read_only);
        sycl::accessor msg_accessor(msg_buf, h, sycl::read_write);
        h.parallel_for(blocksN_range, [=](uint32_t blockIdx) {
          CtrBlock block = {
              .nonce = nonce,
              .iv = iv,
              .counter = htonl(blockIdx + 1)
          };
          aes_accessor[0].encrypt(block.state);
          for(size_t i = 0; i<16; ++i){
              msg_accessor[blockIdx*16 + i] ^= block.data[i];
          }
        });
      });
    }
    if(remain){
        CtrBlock block = {
          .nonce = nonce,
          .iv = iv,
          .counter = htonl(blocksN + 1)
        };
        aes.encrypt(block.state);
        for(size_t i = 0; i<remain; ++i){
            encrypted_message[encrypted_message.size() - remain + i] ^= block.data[i];
        }
    }

    q.wait();
    return encrypted_message;
}

#endif

} // namespace aes
