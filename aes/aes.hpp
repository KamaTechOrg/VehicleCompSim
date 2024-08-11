#pragma once
#include <array>
#include <cstdint>
#include <iostream>

#if SYCL_ENABLED
#include <sycl/sycl.hpp>
#endif

namespace aes {

enum class AesVariant {Aes128, Aes192, Aes256};

using AesState = std::array<std::array<uint8_t, 4>, 4>;

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

#if SYCL_ENABLED
  SYCL_EXTERNAL void encrypt(State&) const;
  SYCL_EXTERNAL void decrypt(State&) const;
#else
  void encrypt(State&) const;
  void decrypt(State&) const;
#endif

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

} // namespace aes
