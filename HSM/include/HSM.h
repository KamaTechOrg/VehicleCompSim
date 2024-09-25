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

    
    enum HASH_ALGORITHM_TYPE
    {
        NoHash,
        SHA1,
        SHA256,
        SHA512,
        Hash_Count,
        SHA3_256,
        SHA3_512,
    };

    class KeyStorage
    {
    private:
        KeyStorage();
        static KeyStorage *instance;
        u_int32_t keyIdCounter = 0;
        static const std::string KeyStorageFileName;

    protected:
        static HSM_STATUS getKeyFromKeyStorage(
            const std::vector<u_char> &myId, 
            u_int32_t keyId, 
            ENCRYPTION_ALGORITHM_TYPE type, 
            std::vector<u_char> &publicKey, 
            std::vector<u_char> &privateKey
        );

    public:
        static KeyStorage &getInstance();
        HSM_STATUS writeToStorage(std::string info);
        ~KeyStorage();

        static HSM_STATUS get_keys(
            const std::vector<u_char> &myId, 
            u_int32_t &keyId, 
            ENCRYPTION_ALGORITHM_TYPE type, 
            int bits = 512
        );

        HSM_STATUS searchInStorage(
            const std::vector<u_char> &myId, 
            u_int32_t &keyId, 
            ENCRYPTION_ALGORITHM_TYPE type, 
            std::vector<u_char> &publicKey, 
            std::vector<u_char> &privateKey
        );
   
    };

    class Algo : public KeyStorage
    {
    public:
        static HSM_STATUS encrypt(
            const std::vector<u_char> &message, 
            std::vector<u_char> &encrypted_message, 
            ENCRYPTION_ALGORITHM_TYPE type, 
            const std::vector<u_char> &myId, 
            u_int32_t keyId
        );  

        static HSM_STATUS decrypt(
            const std::vector<u_char> &message, 
            std::vector<u_char> &decrypted_message, 
            ENCRYPTION_ALGORITHM_TYPE type, 
            const std::vector<u_char> &myId, 
            u_int32_t keyId
        );

        static HSM_STATUS signMessage(
            const std::vector<u_char> &message,
            std::vector<u_char> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            HASH_ALGORITHM_TYPE hashAlg,
            const std::vector<u_char> &myId, 
            u_int32_t keyId
        );

        static HSM_STATUS verify(
            const std::vector<u_char> &message,
            const std::vector<u_char> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            HASH_ALGORITHM_TYPE hashAlg,
            const std::vector<u_char> &myId, 
            u_int32_t keyId
        );
    };
}
