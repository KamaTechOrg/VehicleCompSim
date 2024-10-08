#pragma once

namespace HSMnamespace {

/**
 * @enum HSM_STATUS
 * @brief Status for the HSM module.
 */
enum HSM_STATUS {
  HSM_Good,
  HSM_InvalidAlg,
  HSM_InvalidIdent,
  HSM_NoSuchKey,
  HSM_KeyNotValid,
  HSM_WrongDataSize,
  HSM_WrongPadding,
  HSM_BadOutputBufSize,
  HSM_InternalErr,
  // Add more here
  HSM_Status_Count
};

enum ENCRYPTION_ALGORITHM_TYPE {
      NoAlg,
      RSA,
      AES_128_ECB,
      AES_128_CBC,
      AES_128_CTR,
      AES_192_ECB,
      AES_192_CBC,
      AES_192_CTR,     
      AES_256_ECB,
      AES_256_CBC,
      AES_256_CTR,
      ECC,
      Alg_Count
  };
};