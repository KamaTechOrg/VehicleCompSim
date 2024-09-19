#include <string>
#include "HSM.h"
#include "AES_API.hpp"
#include "HSM_type_enum.hpp"

HSM_STATUS HSM::encrypt(const std::vector<u_char> &message, std::vector<u_char> &encrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> &myId, u_int32_t keyId)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey);
    if (status != HSM_STATUS::SUCCESS)
        return status;
    switch (type)
    {
    case RSA:
        encrypted_message = RSA::encrypt(message, publicKey);


        break;

    case AES:
        encrypted_message = AES::encrypt(message, publicKey);
        break;
    
    default:
        break;
    }
    return HSM_STATUS();
}

HSM_STATUS HSM::decrypt(const std::vector<u_char> &message, std::vector<u_char> &decrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> &myId, u_int32_t keyId)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey);
    if (status != HSM_STATUS::SUCCESS)
        return status;
    switch (type)
    {
    case RSA:
        decrypted_message = RSA::decrypt(message, privateKey);
        break;
    case AES:
        decrypted_message = AES::decrypt(message, privateKey);
        break;

    default:
        break;
    }
    return HSM_STATUS();
}

HSM_STATUS HSM::signMessage(const std::vector<u_char> &message, std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign, const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
    if (status != HSM_STATUS::SUCCESS)
        return status;


    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA3_256) return Signature::sha3_256_sign(message, sigAlg, key);
    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA256) return Signature::sha256_sign(message, sigAlg, key);
    return HSM_STATUS();
}

HSM_STATUS HSM::verify(const std::vector<u_char> &message, const std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign, const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
    if (status != HSM_STATUS::SUCCESS)
        return status;
        
    return HSM_STATUS();
}
