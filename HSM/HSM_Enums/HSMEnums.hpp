#pragma once

namespace HSMns {

/**
 * @enum HSM_STATUS
 * @brief Enum representing the various status codes for the HSM (Hardware Security Module) module.
 */
enum HSM_STATUS {
  HSM_Good,            /**< Operation completed successfully */
  HSM_InvalidAlg,      /**< Invalid algorithm provided */
  HSM_InvalidIdent,    /**< Invalid identifier */
  HSM_NoSuchKey,       /**< No such key found */
  HSM_KeyNotValid,     /**< The key is not valid */
  HSM_WrongDataSize,   /**< Incorrect data size */
  HSM_WrongPadding,    /**< Incorrect padding */
  HSM_BadOutputBufSize,/**< Output buffer size is incorrect */
  HSM_InternalErr,     /**< Internal error in the HSM module */
  HSM_Status_Count     /**< Total count of HSM status codes */
};

/**
 * @enum ENCRYPTION_ALGORITHM_TYPE
 * @brief Enum representing different types of encryption algorithms.
 */
enum ENCRYPTION_ALGORITHM_TYPE {
  NoAlg,         /**< No encryption algorithm */
  RSA,           /**< RSA encryption algorithm */
  AES_128_ECB,   /**< AES 128-bit in ECB mode */
  AES_128_CBC,   /**< AES 128-bit in CBC mode */
  AES_128_CTR,   /**< AES 128-bit in CTR mode */
  AES_192_ECB,   /**< AES 192-bit in ECB mode */
  AES_192_CBC,   /**< AES 192-bit in CBC mode */
  AES_192_CTR,   /**< AES 192-bit in CTR mode */
  AES_256_ECB,   /**< AES 256-bit in ECB mode */
  AES_256_CBC,   /**< AES 256-bit in CBC mode */
  AES_256_CTR,   /**< AES 256-bit in CTR mode */
  ECC,           /**< Elliptic Curve Cryptography (ECC) algorithm */
  Alg_Count      /**< Total count of encryption algorithms */
};

}; // namespace HSMns
