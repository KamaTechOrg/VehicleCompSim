#include "HSM.h"
#include "AES_API.hpp"
#include "RSA_ENC.h"
#include "RSA_KEY.h"
#include <vector>


using namespace HSM;

HSM_STATUS Algo::encrypt(const std::vector<u_int8_t> &message, std::vector<u_int8_t> &encrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const Ident &myId, const KeyId &keyId, bool needPrivilege)
{
    std::vector<u_int8_t> publicKey;
    std::vector<u_int8_t> privateKey;
    HSM_STATUS status = KeyStorage::getInstance().getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey, needPrivilege);
    if (status != HSM_STATUS::HSM_Good)
        return status;

    switch (type)
    {
    case RSA: 
        encrypted_message = RSA_ENC::encrypt(message, publicKey);
        break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR: 
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR:
        return AES::encrypt(message, encrypted_message, publicKey, type);

    default:
        return HSM_STATUS::HSM_InvalidAlg;
    }
    return HSM_STATUS::HSM_Good;
}

HSM_STATUS Algo::decrypt(const std::vector<u_int8_t> &message, std::vector<u_int8_t> &decrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const Ident &myId, const KeyId &keyId)
{
    std::vector<u_int8_t> publicKey;
    std::vector<u_int8_t> privateKey;
    HSM_STATUS status = KeyStorage::getInstance().getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;

    switch (type)
    {
    case RSA:
        decrypted_message = RSA_ENC::decrypt(message, privateKey);
        break;

    case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR: 
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR:
        return AES::decrypt(message, decrypted_message, privateKey, type);

    default:
        return HSM_InvalidAlg;
    }
    return HSM_STATUS();
}

HSM_STATUS HSM::Algo::signMessage(
    const std::vector<u_int8_t> &message, 
    std::vector<u_int8_t> &signature, 
    ENCRYPTION_ALGORITHM_TYPE sigAlg, 
    HASH_ALGORITHM_TYPE hashAlg, 
    const Ident &myId, 
    const KeyId &keyId
){
    std::vector<u_int8_t> publicKey;
    std::vector<u_int8_t> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myId, keyId, sigAlg, publicKey, privateKey);

    if (status != HSM_STATUS::HSM_Good)
        return status;
        
    status = SHA_API::sha(message, hashAlg);

    if (status != HSM_STATUS::HSM_Good)
        return status;
    
    return encrypt(message, signature, sigAlg, myId, keyId);
}

HSM_STATUS HSM::Algo::verify(
    const std::vector<u_int8_t> &message, 
    const std::vector<u_int8_t> &signature, 
    ENCRYPTION_ALGORITHM_TYPE sigAlg,  
    HASH_ALGORITHM_TYPE hashAlg, 
    const Ident &myId, 
    const KeyId &keyId,
    bool needPrivilege
){
    std::vector<u_int8_t> publicKey;
    std::vector<u_int8_t> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myId, keyId, sigAlg, publicKey, privateKey, needPrivilege);
    if (status != HSM_STATUS::HSM_Good)
        return status;
    // status = decrypt(signature, signature, sigAlg, myId, keyId);
    if (status != HSM_STATUS::HSM_Good)
        return status;
    status = SHA_API::sha(message, hashAlg);
    if (status != HSM_STATUS::HSM_Good)
        return status;

    return SHA_API::comperHash(message, signature, hashAlg);
}

