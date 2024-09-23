#include "HSM.h"
#include "AES_API.hpp"
#include "RSA_ENC.h"
#include "RSA_KEY.h"
#include <vector>

using namespace HSM;

const std::string KeyStorage::KeyStorageFileName = "KeyStorage.csv";



HSM_STATUS KeyStorage::get_keys(const std::vector<u_char> &myId, u_int32_t &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    switch (type)
    {
    case RSA:

        return RSA_KEY::generateKeys(publicKey, privateKey, bits);

        break;

    case ECC:

        break;

    case AES:
        break;

    default:
        break;
    }
    return HSM_STATUS();
}

KeyStorage &HSM::KeyStorage::getInstance()
{
    if(instance == nullptr)
        instance = new KeyStorage();
    return *instance;
}

HSM_STATUS KeyStorage::getKeyFromKeyStorage(const std::vector<u_char> &myId, u_int32_t keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey)
{
    return HSM_STATUS();
}


HSM_STATUS Algo::encrypt(const std::vector<u_char> &message, std::vector<u_char> &encrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> &myId, u_int32_t keyId)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = KeyStorage::getInstance().getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;
    switch (type)
    {
    case RSA:
        encrypted_message = RSA_ENC::encrypt(message, publicKey);


        break;

    case AES:
        encrypted_message = AES::encrypt(message, publicKey);
        break;
    
    default:
        break;
    }
    return HSM_STATUS();
}

HSM_STATUS Algo::decrypt(const std::vector<u_char> &message, std::vector<u_char> &decrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> &myId, u_int32_t keyId)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = KeyStorage::getInstance().getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;
    switch (type)
    {
    case RSA:
        decrypted_message = RSA_ENC::decrypt(message, privateKey);
        break;
    case AES:
        decrypted_message = AES::decrypt(message, privateKey);
        break;

    default:
        break;
    }
    return HSM_STATUS();
}

HSM_STATUS Algo::signMessage(const std::vector<u_char> &message, std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign, const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;


    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA3_256) return Signature::sha3_256_sign(message, sigAlg, key);
    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA256) return Signature::sha256_sign(message, sigAlg, key);
    return HSM_STATUS();
}

HSM_STATUS Algo::verify(const std::vector<u_char> &message, const std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign, const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;
        
    return HSM_STATUS();
}
