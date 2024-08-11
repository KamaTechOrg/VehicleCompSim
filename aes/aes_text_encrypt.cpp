#include <cstddef>
#include <cstdint>
#include <execution>

// for htonl
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

#include "aes_text_encrypt.hpp"
#include "aes.hpp"



#if SYCL_ENABLED
#include <sycl/sycl.hpp>
#endif

namespace aes {


/* /////////// ECB mode \\\\\\\\\\\  */

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ecb(Aes<Aes_var> const& aes, std::string const& message){
    std::string encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN); // encrypted_message.size() == N*sizeof(State)
  
  encrypted_message += message; // fill message
  encrypted_message.resize(message.size()+paddingN, paddingN); // fill padding
  using State = typename Aes<Aes_var>::State;
  auto* states_buf = reinterpret_cast<State*>(encrypted_message.data());
  std::for_each_n(std::execution::seq, states_buf, encrypted_message.size()/16, [&](auto& state){
    aes.encrypt(state);
  });
  return encrypted_message;
}


template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::decrypt_ecb(Aes<Aes_var> const& aes, std::string const& encrypted_message) {
  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  std::string message = encrypted_message;
  using State = typename Aes<Aes_var>::State;
  State* states_buf = reinterpret_cast<State*>(message.data());
  std::for_each_n(std::execution::seq, states_buf, message.size()/16, [&](auto& state){
    aes.decrypt(state);
  });

  if(message.back() > 16){
    throw std::invalid_argument("aes invalid padding char");
  }
  message.resize(message.size() - message.back());
  return message;
}


template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ecb(KeyType const& key, std::string const& message){
    return encrypt_ecb(Aes<Aes_var>(key), message);
}

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::decrypt_ecb(KeyType const& key, std::string const& encrypted_message){
    return decrypt_ecb(Aes<Aes_var>(key), encrypted_message);
}

#if SYCL_ENABLED

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ecb(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& message)  {
  std::string encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN); // encrypted_message.size() == N*sizeof(State)
  
  encrypted_message += message; // fill message
  encrypted_message.resize(message.size()+paddingN, paddingN); // fill padding

  size_t blocksN = encrypted_message.size()/16;
  sycl::range<1> blocksN_range{blocksN};
  using State = typename Aes<Aes_var>::State;
  sycl::buffer states_buf(reinterpret_cast<State*>(encrypted_message.data()), blocksN_range);
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


template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::decrypt_ecb(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& encrypted_message) {
  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  std::string message = encrypted_message;

  using State = typename Aes<Aes_var>::State;
  { 
    sycl::range<1> blocksN_range{message.size()/16};
    sycl::buffer states_buf(reinterpret_cast<State*>(message.data()), blocksN_range);
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

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ecb(sycl::queue& q, typename Aes<Aes_var>::KeyType const& key, std::string const& message){
    return AesTextEncrypt<Aes_var>::encrypt_ecb(q, Aes<Aes_var>(key), message);
}

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::decrypt_ecb(sycl::queue& q, typename Aes<Aes_var>::KeyType const& key, std::string const& encrypted_message){
    return AesTextEncrypt<Aes_var>::decrypt_ecb(q, Aes<Aes_var>(key), encrypted_message);
}


#endif



/* /////////// CBC mode \\\\\\\\\\\  */

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_cbc(Aes<Aes_var> const& aes, std::string const& message, std::array<uint8_t, 16> const& iv) {
  using State = typename Aes<Aes_var>::State;
  auto Nb = Aes<Aes_var>::Nb;

  State prev_state;
  for (uint col = 0; col < Nb; ++col) {
    for (uint row = 0; row < Nb ; ++row) {
      prev_state[col][row] = iv[row + col*4];
    }
  }
  State state;
  std::string encrypted_message;
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
        encrypted_message += state[col][row];
      }
    }
    prev_state = state;
    msg_idx += Nb*Nb;
  }
  return encrypted_message;
}


template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::decrypt_cbc(Aes<Aes_var> const& aes, std::string const& encrypted_message, std::array<uint8_t, 16> const& iv)  {
  using State = typename Aes<Aes_var>::State;
  auto Nb = Aes<Aes_var>::Nb;

  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  State prev_state;
  for (uint col = 0; col < Nb; ++col) {
    for (uint row = 0; row < Nb ; ++row) {
      prev_state[col][row] = iv[row + col*4];
    }
  }
  State state;
  std::string message;
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
        message += state[col][row]^prev_state[col][row];
      }
    }
    prev_state = tmp;
    msg_idx += Nb*Nb;
  }
  if(message.back() > 16){
    throw std::invalid_argument(std::string("aes invalid padding char: ") + message.back());
  }
  message.resize(message.size() - message.back());
  return message;
}

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_cbc(typename Aes<Aes_var>::KeyType const& key, std::string const& message, std::array<uint8_t, 16> const& iv){
    return encrypt_cbc(Aes<Aes_var>(key), message, iv);
}

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::decrypt_cbc(typename Aes<Aes_var>::KeyType const& key, std::string const& encrypted_message, std::array<uint8_t, 16> const& iv){
    return decrypt_cbc(Aes<Aes_var>(key), encrypted_message, iv);
}



/* /////////// CTR mode \\\\\\\\\\\  */

struct CtrBlock{
  union {
    AesState state;
    struct {
      std::array<uint8_t, 4> nonce;
      std::array<uint8_t, 8> iv;
      uint32_t counter;
    };
    std::array<uint8_t, 16> data;
  };
};

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ctr(Aes<Aes_var> const& aes, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce){
    size_t blocksN = (message.size() + 15) / 16; 
    std::string encrypted_message = message;
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

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ctr(KeyType const& key, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce){
  return encrypt_ctr(Aes<Aes_var>(key), message, iv, nonce);
}



#if SYCL_ENABLED
template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ctr(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce){
    size_t blocksN = message.size() / 16; //entire blocks
    size_t remain = message.size() % 16;
    std::string encrypted_message = message;
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

template <AesVariant Aes_var>
std::string AesTextEncrypt<Aes_var>::encrypt_ctr(sycl::queue& q, typename Aes<Aes_var>::KeyType const& key, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce){
    return AesTextEncrypt<Aes_var>::encrypt_ctr(q, Aes<Aes_var>(key), message, iv, nonce);
}


#endif

/* /////////// explicit instantiations \\\\\\\\\\\  */

template
class AesTextEncrypt<AesVariant::Aes128>;

template
class AesTextEncrypt<AesVariant::Aes192>;

template
class AesTextEncrypt<AesVariant::Aes256>;


} // namespace aes
