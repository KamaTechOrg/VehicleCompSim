#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <cstring>
#include "aes_consts.hpp"

enum class AesVariant {Aes128, Aes192, Aes256};

template <AesVariant Aes_var>
class Aes {
public:
  static const uint Nb = 4; // number of columns in the state & expanded key
  static const uint BlockSize = Nb*Nb; // number of bytes in block

  // number of rounds in encryption
  static const uint Nr = Aes_var == AesVariant::Aes128 ? 10 
                        :Aes_var == AesVariant::Aes192 ? 12 
                        :       /* AesVariant::Aes256 */ 14;

  // number of columns in a key
  static const uint Nk = Aes_var == AesVariant::Aes128 ? 4
                        :Aes_var == AesVariant::Aes192 ? 6 
                        :       /* AesVariant::Aes256 */ 8; 
  static const uint KeySize = Nk*4; // in bytes
  using KeyType = std::array<uint8_t, KeySize>;
  
  explicit Aes(std::array<uint8_t, KeySize> const &key) { expand_key(key); }
  using State = std::array<std::array<uint8_t, 4>, 4>;

  std::string encrypt_ecb(std::string const& message) const;
  std::string decrypt_ecb(std::string const& encrypted_message) const;

  std::string encrypt_cbc(std::string const& message, std::array<uint8_t, BlockSize> const& iv) const;
  std::string decrypt_cbc(std::string const& encrypted_message, std::array<uint8_t, BlockSize> const& iv) const;

  void encrypt(State&) const;
  void decrypt(State&) const;

private:
  void expand_key(std::array<uint8_t, KeySize> const &key);
  void add_round_key(State &, uint key_index) const;

  void subBytes(State &) const;
  void shiftRows(State &) const;
  void mixColumns(State &) const;

  void inv_subBytes(State &) const;
  void inv_shiftRows(State &) const;
  void inv_mixColumns(State &) const;

private:
  std::array<std::array<uint8_t, 4>, Nb * (Nr + 1)> m_expended_key;
};

template <AesVariant Aes_var>
void print_state(typename Aes<Aes_var>::State const& state){
    for (auto &row :state) {
      for (uint c : row) {
        std::cout << std::hex << c << ' ' << std::flush;
      }
      std::cout << "\n";
    }
    std::cout << "\n";
}

/* /////////////   implementation \\\\\\\\\\\\\\ */

template <AesVariant Aes_var>
std::string Aes<Aes_var>::encrypt_ecb(std::string const& message) const {
  State state;
  std::string encrypted_message;
  uint8_t paddingN = (message.size()%16) ? (16-message.size()%16) : 16;
  encrypted_message.reserve(message.size() + paddingN); // encrypted_message.size() == N*sizeof(State)
  
  encrypted_message += message; // fill message
  encrypted_message.resize(message.size()+paddingN, paddingN); // fill padding
  for(size_t msg_idx = 0; msg_idx<encrypted_message.size(); msg_idx += sizeof(State)){
    auto& state = reinterpret_cast<State&>(encrypted_message[msg_idx]);
    encrypt(state);
  }
  return encrypted_message;
}

template <AesVariant Aes_var>
std::string Aes<Aes_var>::decrypt_ecb(std::string const& encrypted_message) const {
  if(encrypted_message.size() % 16 != 0){
    throw std::invalid_argument("encrypted_message.size() most be N*16 not " + std::to_string(encrypted_message.size()));
  }
  State state;
  std::string message = encrypted_message;
  // message.reserve(encrypted_message.size());
  for(size_t msg_idx = 0; msg_idx<encrypted_message.size(); msg_idx += sizeof(State)){
    auto& state = reinterpret_cast<State&>(message[msg_idx]);
    decrypt(state);
  }
  if(message.back() > 16){
    throw std::invalid_argument("aes invalid padding char");
  }
  message.resize(message.size() - message.back());
  return message;
}


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
  uint8_t padding_idx = 0;
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
      // state.at(col).at(row) ^= m_expended_key.at(key_index*Nb + row).at(col);
    }
  }
}

template <AesVariant Aes_var>  void Aes<Aes_var>::subBytes(State &state) const {
  for (auto &row : state)
    for (uint8_t &byte : row)
      byte = s_box[byte];
}

template <AesVariant Aes_var>  void Aes<Aes_var>::inv_subBytes(State &state) const {
  for (auto &row : state)
    for (uint8_t &byte : row)
      byte = rs_box[byte];
}


template <AesVariant Aes_var>  void Aes<Aes_var>::shiftRows(State &state) const {
  std::array<uint8_t, 4> tmp;
  for (int row = 1; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) tmp[col] = state[col][row];
    int offset = 4-row;
    for (int col = 0; col < 4; ++col) {
      state[(col + offset) % 4][row] = tmp[col];
      // tmp[col] = state[row][(col + offset) % 4];
    }
    // state[row] = tmp;
  }
}

template <AesVariant Aes_var>  void Aes<Aes_var>::inv_shiftRows(State &state) const {
  // std::array<uint8_t, 4> tmp;
  // for (int row = 1; row < 4; ++row) {
  //   int offset = 4 - row;
  //   for (int col = 0; col < 4; ++col) {
  //     tmp[col] = state[row][(col + offset) % 4];
  //   }
  //   state[row] = tmp;
  // }
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
