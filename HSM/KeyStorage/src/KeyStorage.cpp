#include "KeyStorage.h"
#include "RSA_KEY.h"

HSM_STATUS KeyStorage::get_keys(const std::vector<u_char> &myId, u_int32_t &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
    vector<u_char> publicKey;
    vector<u_char> privateKey;
    switch (type)
    {
    case RSA:
    RSA_KEY::generat

        
        break;
    
    default:
        break;
    }
    return HSM_STATUS();
}
std::pair<std::vector<u_char>, std::vector<u_char>> HSM::generateAsymmetricKeys(ENCRYPTION_ALGORITHM_TYPE type, int bits = 512)
{
    if(type == ENCRYPTION_ALGORITHM_TYPE::RSA) return RSA::generateKeys(type, bits);
    // if(type == ENCRYPTION_ALGORITHM_TYPE::ECC) return ECC::generateKeys(type, bits);
    return std::pair<std::vector<u_char>, std::vector<u_char>> ();
}
std::vector<u_char> HSM::generateSymmetricKey(ENCRYPTION_ALGORITHM_TYPE  type, int bits = 128)
{
    // if(type == ENCRYPTION_ALGORITHM_TYPE::AES) return AES::generateKey(type);
    return std::vector<u_char>();
}

HSM_STATUS KeyStorage::getKeyFromKeyStorage(const std::vector<u_char> &myId, u_int32_t keyId, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey)
{
    return HSM_STATUS();
}