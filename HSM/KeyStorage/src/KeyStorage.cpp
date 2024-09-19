#include "KeyStorage.h"
#include "RSA_KEY.h"

HSM_STATUS KeyStorage::get_keys(const std::vector<u_char> &myId, u_int32_t &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
    vector<u_char> publicKey;
    vector<u_char> privateKey;
    switch (type)
    {
    case RSA:

        return RSA_KEY::generateKeys(publicKey, privateKey, type, bits);

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

HSM_STATUS KeyStorage::getKeyFromKeyStorage(const std::vector<u_char> &myId, u_int32_t keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey)
{
    return HSM_STATUS();
}
