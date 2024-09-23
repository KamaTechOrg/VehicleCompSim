#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include "HSMStatusEnum.hpp"

namespace HSM
{

    enum ENCRYPTION_ALGORITHM_TYPE
    {
        NoAlg,
        RSA,
        AES,
        ECC,
        Alg_Count
    };

    class KeyStorage
    {
    public:
        static HSM_STATUS get_keys(const std::vector<u_char> &myId, u_int32_t &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits = 512);
        static KeyStorage &getInstance();

    protected:
        static HSM_STATUS getKeyFromKeyStorage(const std::vector<u_char> &myId, u_int32_t keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey);

    private:
        KeyStorage()
        {
            std::cout << "KeyStorage constructor" << std::endl;
        }
        static KeyStorage *instance;

        ~KeyStorage()
        {
            std::cout << "KeyStorage destructor" << std::endl;
        }
        static const std::string KeyStorageFileName;
    };

    class Algo : public KeyStorage
    {
    public:
        static HSM_STATUS encrypt(const std::vector<u_char> &message, std::vector<u_char> &encrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> &myId, u_int32_t keyId);

        static HSM_STATUS decrypt(const std::vector<u_char> &message, std::vector<u_char> &decrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> &myId, u_int32_t keyId);

        static HSM_STATUS signMessage(
            const std::vector<u_char> &message,
            std::vector<u_char> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            ENCRYPTION_ALGORITHM_TYPE hashAlg,
            const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign,
            const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash);

        static HSM_STATUS verify(
            const std::vector<u_char> &message,
            const std::vector<u_char> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            ENCRYPTION_ALGORITHM_TYPE hashAlg,
            const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign,
            const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash);
    };
}
