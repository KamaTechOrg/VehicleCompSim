#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>
#include <sys/types.h>
#include "aes_consts.hpp"

enum class AesVariant {Aes128, Aes194, Aes256};

template <AesVariant Aes_var> 
class Aes {
  static const uint Nb = 4; // number of columns in the state & expanded key

  // number of rounds in encryption
  static const uint Nr = Aes_var == AesVariant::Aes128 ? 10 
                        :Aes_var == AesVariant::Aes194 ? 12 
                        : 14; /* AesVariant::Aes194 */

  // number of columns in a key
  static const uint Nk = Aes_var == AesVariant::Aes128 ? 4
                        :Aes_var == AesVariant::Aes194 ? 6 
                        : 8; /* AesVariant::Aes256 */
  static const uint KeySize = Nk*4; // in bytes
public:
  explicit Aes(std::array<uint8_t, KeySize> const &key) { expand_key(key); }
  using State = std::array<std::array<uint8_t, 4>, 4>;

  std::string encript(std::string const& message) const;
  std::string decript(std::string const& encripted_message) const;

  void encript(State&) const;
  void decript(State&) const;

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

#include <iostream>
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
std::string Aes<Aes_var>::encript(std::string const& message) const {
  State state;
  std::string encripted_message;
  encripted_message.reserve(message.size());
  size_t msg_idx = 0;
  while(msg_idx < message.size()){
    for (int col = 0; col < Nb && msg_idx < message.size(); ++col) {
      for (int row = 0; row < Nb ; ++row) {
        if(msg_idx + row + col*4 < message.size()){
          state[row][col] = message[msg_idx + row + col*4];
        } else {
          // TODO: padding
          break;
        }
      }
    }
    encript(state);
    for (int col = 0; col < Nb && msg_idx < message.size(); ++col) {
      for (int row = 0; row < Nb; ++row) {
        encripted_message += state[row][col];
      }
    }
    msg_idx += Nb*Nb;
  }
  return encripted_message;
}


template <AesVariant Aes_var>
std::string Aes<Aes_var>::decript(std::string const& encripted_message) const {
  State state;
  std::string message;
  message.reserve(encripted_message.size());
  size_t msg_idx = 0;
  while(msg_idx < encripted_message.size()){
    for (int col = 0; col < Nb && msg_idx < encripted_message.size(); ++col) {
      for (int row = 0; row < Nb ; ++row) {
        if(msg_idx + row + col*4 < encripted_message.size()){
          state[row][col] = encripted_message[msg_idx + row + col*4];
        } else {
          // TODO: padding
          break;
        }
      }
    }
    decript(state);
    for (int col = 0; col < Nb && msg_idx < encripted_message.size(); ++col) {
      for (int row = 0; row < Nb; ++row) {
        message += state[row][col];
      }
    }
    msg_idx += Nb*Nb;
  }
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
void Aes<Aes_var>::encript(State & state) const {
  uint key_index = 0;
  add_round_key(state, key_index);
  ++key_index;

  for (size_t round = 0; round < Nr; ++round) {
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
void Aes<Aes_var>::decript(State & state) const {
  size_t key_index = m_expended_key.size()/Nb;
  add_round_key(state, key_index);
  --key_index;
  inv_shiftRows(state);
  inv_subBytes(state);
  for (size_t round = 0; round < Nr; ++round) {
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
      state[col][row] ^= m_expended_key[key_index*Nb + row][col];
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
    int& offset = row;
    for (int col = 0; col < 4; ++col) {
      tmp[col] = state[row][(col + offset) % 4];
    }
    state[row] = tmp;
  }
}

template <AesVariant Aes_var>  void Aes<Aes_var>::inv_shiftRows(State &state) const {
  std::array<uint8_t, 4> tmp;
  for (int row = 1; row < 4; ++row) {
    int offset = 4 - row;
    for (int col = 0; col < 4; ++col) {
      tmp[col] = state[row][(col + offset) % 4];
    }
    state[row] = tmp;
  }
}

template <AesVariant Aes_var>  void Aes<Aes_var>::mixColumns(State &state) const {
  for (size_t col = 0; col < 4; ++col) {
    uint8_t tmp[4];
    tmp[0] = galo_mul_2[state[col][0]] ^ galo_mul_3[state[col][1]] ^
             state[col][2] ^ state[col][3];
    tmp[1] = state[col][0] ^ galo_mul_2[state[col][1]] ^
             galo_mul_3[state[col][2]] ^ state[col][3];
    tmp[2] = state[col][0] ^ state[col][1] ^ galo_mul_2[state[col][2]] ^
             galo_mul_3[state[col][3]];
    tmp[3] = galo_mul_3[state[col][0]] ^ state[col][1] ^ state[col][2] ^
             galo_mul_2[state[col][3]];

    for (size_t row = 0; row < 4; ++row)
      state[col][row] = tmp[row];
  }
}

template <AesVariant Aes_var>  void Aes<Aes_var>::inv_mixColumns(State &state) const {
  for (size_t col = 0; col < 4; ++col) {
    uint8_t tmp[4];
    tmp[0] = galo_mul_14[state[col][0]] ^ galo_mul_11[state[col][1]] ^
             galo_mul_13[state[col][2]] ^ galo_mul_9[state[col][3]];
    tmp[1] = galo_mul_9[state[col][0]] ^ galo_mul_14[state[col][1]] ^
             galo_mul_11[state[col][2]] ^ galo_mul_13[state[col][3]];
    tmp[2] = galo_mul_13[state[col][0]] ^ galo_mul_9[state[col][1]] ^
             galo_mul_14[state[col][2]] ^ galo_mul_11[state[col][3]];
    tmp[3] = galo_mul_11[state[col][0]] ^ galo_mul_13[state[col][1]] ^
             galo_mul_9[state[col][2]] ^ galo_mul_14[state[col][3]];

    for (size_t row = 0; row < 4; ++row)
      state[col][row] = tmp[row];
  }
}
