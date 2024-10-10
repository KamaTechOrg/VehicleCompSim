#pragma once
#include <array>
#include <cstdint>
#include <iostream>

#if SYCL_ENABLED
#include <sycl/sycl.hpp>
#endif

namespace aes {

/**
 * @enum AesVariant
 * @brief Enumeration of AES algorithm variants (128-bit, 192-bit, 256-bit).
 */
enum class AesVariant {
    NoAesVariant, /**< No AES variant specified */
    Aes128,       /**< AES-128 variant */
    Aes192,       /**< AES-192 variant */
    Aes256        /**< AES-256 variant */
};

/**
 * @class AES_block_encrypt
 * @brief Base class for AES block encryption and decryption.
 * 
 * This class provides the basic structure for AES encryption and decryption, 
 * defining a state matrix and virtual encrypt/decrypt methods that are 
 * implemented by derived classes.
 */
class AES_block_encrypt {
public:
  static const uint Nb = 4;        /**< Number of columns in the state and expanded key */
  static const uint BlockSize = Nb*Nb; /**< Number of bytes in an AES block */

  /**
   * @brief Defines the state matrix used in AES encryption.
   * 
   * The state is a 4x4 matrix of bytes (uint8_t), where each byte represents
   * a value in the AES block.
   */
  using State = std::array<std::array<uint8_t, Nb>, Nb>;

  virtual ~AES_block_encrypt() = default;

#if SYCL_ENABLED
  /**
   * @brief Encrypts a block of data (state) using AES (SYCL-enabled version).
   * 
   * @param state The state matrix representing the data to be encrypted.
   */
  SYCL_EXTERNAL virtual void encrypt(State&) const;

  /**
   * @brief Decrypts a block of data (state) using AES (SYCL-enabled version).
   * 
   * @param state The state matrix representing the data to be decrypted.
   */
  SYCL_EXTERNAL virtual void decrypt(State&) const;
#else
  /**
   * @brief Encrypts a block of data (state) using AES.
   * 
   * @param state The state matrix representing the data to be encrypted.
   */
  virtual void encrypt(State&) const = 0;

  /**
   * @brief Decrypts a block of data (state) using AES.
   * 
   * @param state The state matrix representing the data to be decrypted.
   */
  virtual void decrypt(State&) const = 0;
#endif

};

/**
 * @class AES_block_encrypt_impl
 * @brief AES block encryption implementation for a specific AES variant (128-bit, 192-bit, 256-bit).
 * 
 * This class implements AES encryption and decryption for the specified AES variant 
 * and provides methods for key expansion, round operations, and other internal 
 * AES transformations.
 * 
 * @tparam Aes_var The AES variant to be used (Aes128, Aes192, or Aes256).
 */
template <AesVariant Aes_var>
class AES_block_encrypt_impl : public AES_block_encrypt {
public:

  /**
   * @brief Number of rounds in the encryption/decryption process for the given AES variant.
   */
  static const uint Nr = Aes_var == AesVariant::Aes128 ? 10 
                        :Aes_var == AesVariant::Aes192 ? 12 
                        :       /* AesVariant::Aes256 */ 14;

  /**
   * @brief Number of columns in the encryption key for the given AES variant.
   */
  static const uint Nk = Aes_var == AesVariant::Aes128 ? 4
                        :Aes_var == AesVariant::Aes192 ? 6 
                        :       /* AesVariant::Aes256 */ 8; 
  static const uint KeySize = Nk*4; /**< Size of the key in bytes */

  using KeyType = std::array<uint8_t, KeySize>; /**< Type definition for the AES key */
  using State = std::array<std::array<uint8_t, 4>, 4>; /**< Redefinition of the state matrix */

  /**
   * @brief Constructor that initializes the AES encryption implementation with the provided key.
   * 
   * @param key The encryption key used for AES block encryption.
   */
  explicit AES_block_encrypt_impl(KeyType const& key);

#if SYCL_ENABLED
  /**
   * @brief Encrypts a block of data (state) using the specified AES variant (SYCL-enabled version).
   * 
   * @param state The state matrix representing the data to be encrypted.
   */
  SYCL_EXTERNAL void encrypt(State&) const override;

  /**
   * @brief Decrypts a block of data (state) using the specified AES variant (SYCL-enabled version).
   * 
   * @param state The state matrix representing the data to be decrypted.
   */
  SYCL_EXTERNAL void decrypt(State&) const override;
#else
  /**
   * @brief Encrypts a block of data (state) using the specified AES variant.
   * 
   * @param state The state matrix representing the data to be encrypted.
   */
  void encrypt(State&) const override;

  /**
   * @brief Decrypts a block of data (state) using the specified AES variant.
   * 
   * @param state The state matrix representing the data to be decrypted.
   */
  void decrypt(State&) const override;
#endif

private:
  /**
   * @brief Expands the encryption key into an array of round keys.
   * 
   * @param key The encryption key to expand.
   */
  void expand_key(KeyType const &key);

  /**
   * @brief Adds a round key to the state matrix.
   * 
   * @param state The state matrix to which the round key will be added.
   * @param key_index The index of the round key in the expanded key schedule.
   */
  void add_round_key(State &, uint key_index) const;

  /**
   * @brief Substitutes bytes in the state matrix using the AES S-box.
   * 
   * @param state The state matrix to be transformed.
   */
  void subBytes(State &) const;

  /**
   * @brief Shifts rows in the state matrix according to AES specifications.
   * 
   * @param state The state matrix to be transformed.
   */
  void shiftRows(State &) const;

  /**
   * @brief Mixes columns in the state matrix using AES mix columns transformation.
   * 
   * @param state The state matrix to be transformed.
   */
  void mixColumns(State &) const;

  /**
   * @brief Inverse substitute bytes in the state matrix using the AES inverse S-box.
   * 
   * @param state The state matrix to be transformed.
   */
  void inv_subBytes(State &) const;

  /**
   * @brief Inversely shifts rows in the state matrix according to AES specifications.
   * 
   * @param state The state matrix to be transformed.
   */
  void inv_shiftRows(State &) const;

  /**
   * @brief Inversely mixes columns in the state matrix using AES inverse mix columns transformation.
   * 
   * @param state The state matrix to be transformed.
   */
  void inv_mixColumns(State &) const;

private:
  std::array<std::array<uint8_t, 4>, Nb * (Nr + 1)> m_expended_key; /**< Expanded key schedule */
};

using AES_block_encrypt_128 = AES_block_encrypt_impl<AesVariant::Aes128>; /**< AES-128 block encryption */
using AES_block_encrypt_192 = AES_block_encrypt_impl<AesVariant::Aes192>; /**< AES-192 block encryption */
using AES_block_encrypt_256 = AES_block_encrypt_impl<AesVariant::Aes256>; /**< AES-256 block encryption */

} // namespace aes
