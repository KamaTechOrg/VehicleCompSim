enum class HSM_STATUS{
    SUCCESS_GEN_KYE,
    FAIL_GEN_KYE, 
    SUCCESS_ENCRYPT, // start of encryption status
    FAIL_ENCRYPT,
    SUCCESS_DECRYPT, // start of decryption status
    FAIL_DECRYPT,
    SUCCESS_SIGN,   // start of signing status
    FAIL_SIGN,
    SUCCESS_VERIFY,  // start of verification status
    FAIL_VERIFY,
    INVALID_KYE,     // start of get key status
    SUCCESS_GET_KEY
};