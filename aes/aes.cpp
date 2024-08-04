#include <algorithm>
#include <execution>

#include "aes.hpp"
#include "aes_consts.hpp"


/* /////////////   implementation \\\\\\\\\\\\\\  */

template <AesVariant Aes_var>
std::string Aes<Aes_var>::encrypt_ecb(std::string const& message) const {
  std::string encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN); // encrypted_message.size() == N*sizeof(State)
  
  encrypted_message += message; // fill message
  encrypted_message.resize(message.size()+paddingN, paddingN); // fill padding
  State* states_buf = reinterpret_cast<State*>(encrypted_message.data());
  std::for_each_n(std::execution::par, states_buf, encrypted_message.size()/16, [this](auto& state){
    encrypt(state);
  });
  return encrypted_message;
}

template <AesVariant Aes_var>
std::string Aes<Aes_var>::decrypt_ecb(std::string const& encrypted_message) const {
  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  std::string message = encrypted_message;

  State* states_buf = reinterpret_cast<State*>(message.data());
  std::for_each_n(std::execution::par, states_buf, message.size()/16, [this](auto& state){
    decrypt(state);
  });

  if(message.back() > 16){
    throw std::invalid_argument("aes invalid padding char");
  }
  message.resize(message.size() - message.back());
  return message;
}


#ifdef SYCL_ENABELED

template <AesVariant Aes_var>
std::string Aes<Aes_var>::encrypt_ecb(sycl::queue& q, std::string const& message) const {
  std::string encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN); // encrypted_message.size() == N*sizeof(State)
  
  encrypted_message += message; // fill message
  encrypted_message.resize(message.size()+paddingN, paddingN); // fill padding

  size_t blocksN = encrypted_message.size()/16;
  sycl::range<1> blocksN_range{blocksN};
  sycl::buffer states_buf(reinterpret_cast<State*>(encrypted_message.data()), blocksN_range);
  sycl::buffer this_buf(this, sycl::range<1>{1});
  q.submit([&](sycl::handler &h){
    sycl::accessor this_accessor(this_buf, h, sycl::read_only);
    sycl::accessor states_accessor(states_buf, h, sycl::read_write);
    h.parallel_for(blocksN_range, [=](auto i) {
       this_accessor[0].encrypt(states_accessor[i]);
    });
  });
  q.wait();
  return encrypted_message;
}


template <AesVariant Aes_var>
std::string Aes<Aes_var>::decrypt_ecb(sycl::queue& q, std::string const& encrypted_message) const {
  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  std::string message = encrypted_message;



  sycl::range<1> blocksN_range{message.size()/16};
  sycl::buffer states_buf(reinterpret_cast<State*>(message.data()), blocksN_range);
  sycl::buffer this_buf(this, sycl::range<1>{1});
  q.submit([&](sycl::handler &h){
    sycl::accessor this_accessor(this_buf, h, sycl::read_only);
    sycl::accessor states_accessor(states_buf, h, sycl::read_write);
    h.parallel_for(blocksN_range, [=](auto i) {
       this_accessor[0].decrypt(states_accessor[i]);
    });
  });
  q.wait();

  if(message.back() > 16){
    throw std::invalid_argument("aes invalid padding char");
  }
  message.resize(message.size() - message.back());
  return message;
}

#endif


template <AesVariant Aes_var>
std::string Aes<Aes_var>::encrypt_cbc(std::string const& message, std::array<uint8_t, BlockSize> const& iv) const {
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
    encrypt(state);
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
std::string Aes<Aes_var>::decrypt_cbc(std::string const& encrypted_message, std::array<uint8_t, BlockSize> const& iv) const {
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
    for (int col = 0; col < Nb && msg_idx < encrypted_message.size(); ++col) {
      for (int row = 0; row < Nb ; ++row) {
          state[col][row] = encrypted_message[msg_idx + row + col*4];
      }
    }
    auto tmp = state;
    decrypt(state);
    for (int col = 0; col < Nb && msg_idx < encrypted_message.size(); ++col) {
      for (int row = 0; row < Nb; ++row) {
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
void Aes<Aes_var>::expand_key(std::array<uint8_t, KeySize> const &key) {
  for (size_t i = 0; i < Nk; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      m_expended_key[i][j] = key[i * 4 + j];
    }
  }
  uint32_t rcon = 1;
  for (uint key_row = Nk; key_row < m_expended_key.size(); ++key_row) {
    m_expended_key[key_row] = m_expended_key[key_row - 1];
    if (key_row % Nk == 0) {
      uint8_t first = m_expended_key[key_row][0];
      for (uint b = 0; b < 3; ++b)
        m_expended_key[key_row][b] = s_box[m_expended_key[key_row][b + 1]];
      m_expended_key[key_row][3] = s_box[first];
      m_expended_key[key_row][0] ^= rcon;
      rcon = (rcon << 1) ^ (0x11b & -(rcon >> 7));
    } else if( Nk > 6 && key_row % Nk == 4 ) {
      for(uint8_t& b: m_expended_key[key_row])
        b = s_box[b];
    }
    reinterpret_cast<uint32_t &>(m_expended_key[key_row]) ^=
        reinterpret_cast<uint32_t &>(m_expended_key[key_row - Nk]);
  }
}

template <AesVariant Aes_var> 
void Aes<Aes_var>::encrypt(State & state) const {
  uint key_index = 0;
  add_round_key(state, key_index);
  ++key_index;

  for (size_t round = 0; round < Nr-1; ++round) {
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    add_round_key(state, key_index);
    ++key_index;
  }
  subBytes(state);
  shiftRows(state);
  add_round_key(state, key_index);
}

template <AesVariant Aes_var> 
void Aes<Aes_var>::decrypt(State & state) const {
  size_t key_index = m_expended_key.size()/Nb - 1;
  add_round_key(state, key_index);
  --key_index;
  inv_shiftRows(state);
  inv_subBytes(state);
  for (size_t round = 0; round < Nr-1; ++round) {
    add_round_key(state, key_index);
    --key_index;
    inv_mixColumns(state);
    inv_shiftRows(state);
    inv_subBytes(state);
  }
  add_round_key(state, key_index);
}

template <AesVariant Aes_var> 
void Aes<Aes_var>::add_round_key(State &state, uint key_index) const {
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      state[row][col] ^= m_expended_key[key_index*Nb + row][col];
    }
  }
}

template <AesVariant Aes_var>  
void Aes<Aes_var>::subBytes(State &state) const {
  for (auto &row : state)
    for (uint8_t &byte : row)
      byte = s_box[byte];
}

template <AesVariant Aes_var>  void Aes<Aes_var>::inv_subBytes(State &state) const {
  for (auto &row : state)
    for (uint8_t &byte : row)
      byte = rs_box[byte];
}


template <AesVariant Aes_var>  
void Aes<Aes_var>::shiftRows(State &state) const {
  std::array<uint8_t, 4> tmp;
  for (int row = 1; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) tmp[col] = state[col][row];
    int offset = 4-row;
    for (int col = 0; col < 4; ++col) {
      state[(col + offset) % 4][row] = tmp[col];
    }
  }
}

template <AesVariant Aes_var>  void Aes<Aes_var>::inv_shiftRows(State &state) const {
  std::array<uint8_t, 4> tmp;
  for (int row = 1; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) tmp[col] = state[col][row];
    int offset = row;
    for (int col = 0; col < 4; ++col) {
      state[(col + offset) % 4][row] = tmp[col];
    }
  }
}

template <AesVariant Aes_var>  void Aes<Aes_var>::mixColumns(State &state) const {
  State tmp;
  for (size_t col = 0; col < 4; ++col) {
    tmp[col][0] = galo_mul_2[state[col][0]] ^ galo_mul_3[state[col][1]] ^
             state[col][2] ^ state[col][3];
    tmp[col][1] = state[col][0] ^ galo_mul_2[state[col][1]] ^
             galo_mul_3[state[col][2]] ^ state[col][3];
    tmp[col][2] = state[col][0] ^ state[col][1] ^ galo_mul_2[state[col][2]] ^
             galo_mul_3[state[col][3]];
    tmp[col][3] = galo_mul_3[state[col][0]] ^ state[col][1] ^ state[col][2] ^
             galo_mul_2[state[col][3]];
  }
  state = tmp;
}

template <AesVariant Aes_var>  void Aes<Aes_var>::inv_mixColumns(State &state) const {
  State tmp;
  for (size_t col = 0; col < 4; ++col) {
    tmp[col][0] = galo_mul_14[state[col][0]] ^ galo_mul_11[state[col][1]] ^
             galo_mul_13[state[col][2]] ^ galo_mul_9[state[col][3]];
    tmp[col][1] = galo_mul_9[state[col][0]] ^ galo_mul_14[state[col][1]] ^
             galo_mul_11[state[col][2]] ^ galo_mul_13[state[col][3]];
    tmp[col][2] = galo_mul_13[state[col][0]] ^ galo_mul_9[state[col][1]] ^
             galo_mul_14[state[col][2]] ^ galo_mul_11[state[col][3]];
    tmp[col][3] = galo_mul_11[state[col][0]] ^ galo_mul_13[state[col][1]] ^
             galo_mul_9[state[col][2]] ^ galo_mul_14[state[col][3]];
  }
  state = tmp;
}


/* /////////// explicit instantiations \\\\\\\\\\\  */

template
class Aes<AesVariant::Aes128>;

template
class Aes<AesVariant::Aes192>;

template
class Aes<AesVariant::Aes256>;
