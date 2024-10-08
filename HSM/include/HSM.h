#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <memory>
#include "HSMEnums.hpp"
#include "SHA_API.h"

namespace HSMnamespace
{
    typedef uint32_t KeyId;
    class Ident
    {
    private:
        std::vector<u_int8_t> id;

    public:
        Ident();
        Ident(const std::string &str);
        std::string toString() const;
        HSM_STATUS compareID(const Ident &other);
    };

    class KeyStorage
    {
    private:
        u_int32_t keyIdCounter = 0;
        static const std::string KeyStorageFileName;
        std::vector<u_int8_t> keyForKek;
        ENCRYPTION_ALGORITHM_TYPE kekAlgType;
        HSM_STATUS writeToStorage(std::string info);

    protected:
        KeyStorage(ENCRYPTION_ALGORITHM_TYPE kekAlgorithmType, std::string stringKeyForKek);
        ~KeyStorage();
        HSM_STATUS getKeyFromKeyStorage(
            const Ident &myId,
            const KeyId &keyId,
            ENCRYPTION_ALGORITHM_TYPE type,
            std::vector<u_int8_t> &publicKey,
            std::vector<u_int8_t> &privateKey,
            bool needPrivilege = true);

    public:
        HSM_STATUS create_key_and_get_id(
            const Ident &myId,
            KeyId &keyId,
            ENCRYPTION_ALGORITHM_TYPE type,
            int bits = 512);
    };

    class HSM : public KeyStorage
    {
    private:
        static std::unique_ptr<HSM> instance;        
        
        explicit HSM(ENCRYPTION_ALGORITHM_TYPE kekAlgorithmType, std::string stringKeyForKek): KeyStorage(kekAlgorithmType, stringKeyForKek) {}


    public:
        friend HSM &getInstance();
        static void resetInstance();
        HSM_STATUS encrypt(
            const std::vector<u_int8_t> &message,
            std::vector<u_int8_t> &encrypted_message,
            ENCRYPTION_ALGORITHM_TYPE type,
            const Ident &myId,
            const KeyId &keyId,
            bool needPrivilege = true);

        HSM_STATUS decrypt(
            const std::vector<u_int8_t> &message,
            std::vector<u_int8_t> &decrypted_message,
            ENCRYPTION_ALGORITHM_TYPE type,
            const Ident &myId,
            const KeyId &keyId);

        HSM_STATUS signMessage(
            const std::vector<u_int8_t> &message,
            std::vector<u_int8_t> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            HASH_ALGORITHM_TYPE hashAlg,
            const Ident &myId,
            const KeyId &keyId);

        HSM_STATUS verify(
            const std::vector<u_int8_t> &message,
            const std::vector<u_int8_t> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            HASH_ALGORITHM_TYPE hashAlg,
            const Ident &myId,
            const KeyId &keyId,
            bool needPrivilege = true);
    };

    HSM& getInstance();
}
