#include <algorithm>
// #include <execution>

#include "aes.hpp"
#include "aes_consts.hpp"

namespace aes {


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

} // namespace aes
