#pragma once
#include <vector>
#include "AES_block_encrypt.hpp"

#if SYCL_ENABLED
#include <sycl/sycl.hpp>
#endif

namespace aes {

/**
 * @class AES_ChainingStream
 * @brief Provides AES block encryption modes such as ECB, CBC, and CTR.
 * 
 * This class offers static methods for encrypting and decrypting messages using different
 * AES block encryption modes, including ECB (Electronic Codebook), CBC (Cipher Block Chaining),
 * and CTR (Counter Mode). It supports both standard and SYCL-enabled modes.
 */
class AES_ChainingStream {
public:

    /**
     * @brief Encrypts a message using AES in ECB (Electronic Codebook) mode.
     * 
     * @param aes Reference to an AES block encryption object.
     * @param message The message to encrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the encrypted message.
     */
    static std::vector<uint8_t> encrypt_ecb(AES_block_encrypt const& aes, std::vector<uint8_t> const& message);

    /**
     * @brief Decrypts an encrypted message using AES in ECB (Electronic Codebook) mode.
     * 
     * @param aes Reference to an AES block encryption object.
     * @param encrypted_message The encrypted message to decrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the decrypted message.
     */
    static std::vector<uint8_t> decrypt_ecb(AES_block_encrypt const& aes, std::vector<uint8_t> const& encrypted_message);

#if SYCL_ENABLED
    /**
     * @brief Encrypts a message using AES in ECB (Electronic Codebook) mode with SYCL support.
     * 
     * @param q The SYCL queue to execute the encryption operation.
     * @param aes Reference to an AES block encryption object.
     * @param message The message to encrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the encrypted message.
     */
    static std::vector<uint8_t> encrypt_ecb(sycl::queue& q, AES_block_encrypt const& aes, std::vector<uint8_t> const& message);

    /**
     * @brief Decrypts an encrypted message using AES in ECB (Electronic Codebook) mode with SYCL support.
     * 
     * @param q The SYCL queue to execute the decryption operation.
     * @param aes Reference to an AES block encryption object.
     * @param encrypted_message The encrypted message to decrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the decrypted message.
     */
    static std::vector<uint8_t> decrypt_ecb(sycl::queue& q, AES_block_encrypt const& aes, std::vector<uint8_t> const& encrypted_message);
#endif

    /**
     * @brief Encrypts a message using AES in CBC (Cipher Block Chaining) mode.
     * 
     * @param aes Reference to an AES block encryption object.
     * @param iv The initialization vector (IV), represented as an array of 16 bytes.
     * @param message The message to encrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the encrypted message.
     */
    static std::vector<uint8_t> encrypt_cbc(AES_block_encrypt const& aes, std::array<uint8_t, 16> const& iv, std::vector<uint8_t> const& message);

    /**
     * @brief Decrypts an encrypted message using AES in CBC (Cipher Block Chaining) mode.
     * 
     * @param aes Reference to an AES block encryption object.
     * @param iv The initialization vector (IV), represented as an array of 16 bytes.
     * @param encrypted_message The encrypted message to decrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the decrypted message.
     */
    static std::vector<uint8_t> decrypt_cbc(AES_block_encrypt const& aes, std::array<uint8_t, 16> const& iv, std::vector<uint8_t> const& encrypted_message);

    /**
     * @brief Encrypts a message using AES in CTR (Counter) mode.
     * 
     * @param aes Reference to an AES block encryption object.
     * @param iv The initialization vector (IV), represented as an array of 8 bytes.
     * @param nonce The nonce value, represented as an array of 4 bytes.
     * @param message The message to encrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the encrypted message.
     */
    static std::vector<uint8_t> encrypt_ctr(AES_block_encrypt const& aes, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce, std::vector<uint8_t> const& message);

#if SYCL_ENABLED
    /**
     * @brief Encrypts a message using AES in CTR (Counter) mode with SYCL support.
     * 
     * @param q The SYCL queue to execute the encryption operation.
     * @param aes Reference to an AES block encryption object.
     * @param iv The initialization vector (IV), represented as an array of 8 bytes.
     * @param nonce The nonce value, represented as an array of 4 bytes.
     * @param message The message to encrypt, represented as a vector of bytes.
     * @return A vector of bytes representing the encrypted message.
     */
    static std::vector<uint8_t> encrypt_ctr(sycl::queue& q, AES_block_encrypt const& aes, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce, std::vector<uint8_t> const& message);
#endif
};

} // namespace aes
