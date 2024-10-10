/**
 * @file HSM.h
 * @brief Header file for the HSM (Hardware Security Module) classes and related functionality.
 */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <memory>
#include "HSMEnums.hpp"
#include "SHA_API.h"

namespace HSMns
{
    typedef uint32_t KeyId;

    /**
     * @class Ident. this is a poc class, in realty it should incode also the id of the program
     * @brief Class representing an identifier used for key management in the HSM.
     */
    class Ident
    {
    private:
        std::vector<u_int8_t> id; ///< Vector storing the identifier as bytes.

    public:
        /**
         * @brief Default constructor for Ident.
         */
        Ident();

        /**
         * @brief Constructor that initializes Ident with a string.
         * @param str The string to convert to an identifier.
         */
        explicit Ident(const std::string &str);

        /**
         * @brief Equality operator for comparing two Ident objects.
         * @param other The other Ident object to compare.
         * @return True if both identifiers are equal, false otherwise.
         */
        bool operator==(const Ident &other) const = default;

        /**
         * @brief Converts the identifier to a string representation.
         * @return String representation of the identifier.
         */
        std::string toString() const;

        /**
         * @brief Compares this identifier with another identifier.
         * @param other The other Ident object to compare.
         * @return Status of the comparison as HSM_STATUS.
         */
        HSM_STATUS compareID(const Ident &other) const;
    };

    /**
     * @class KeyStorage
     * @brief Class responsible for managing key storage within the HSM.
     */
    class KeyStorage
    {
    private:
        u_int32_t keyIdCounter = 0; ///< Counter for generating unique key IDs.
        static const std::string KeyStorageFileName; ///< File name for key storage.
        std::vector<u_int8_t> keyForKek; ///< Key encryption key (KEK).
        ENCRYPTION_ALGORITHM_TYPE kekAlgType; ///< Algorithm type for KEK.

        /**
         * @brief Writes information to the key storage.
         * @param info Information to write.
         * @return Status of the operation as HSM_STATUS.
         */
        HSM_STATUS writeToStorage(std::string info);

    protected:
        /**
         * @brief Constructor for KeyStorage.
         * @param kekAlgorithmType The encryption algorithm type for the KEK.
         * @param stringKeyForKek The key used for encrypting the KEK.
         */
        explicit KeyStorage(ENCRYPTION_ALGORITHM_TYPE kekAlgorithmType, std::string stringKeyForKek);

        /**
         * @brief Destructor for KeyStorage.
         */
        ~KeyStorage();

        /**
         * @brief Retrieves a key from key storage.
         * @param myId Identifier for the key.
         * @param keyId The ID of the key.
         * @param type The encryption algorithm type.
         * @param publicKey Vector to store the public key.
         * @param privateKey Vector to store the private key.
         * @param needPrivilege Whether privilege is required for the operation (default: true).
         * @return Status of the operation as HSM_STATUS.
         */
        HSM_STATUS getKeyFromKeyStorage(
            const Ident &myId,
            const KeyId &keyId,
            ENCRYPTION_ALGORITHM_TYPE type,
            std::vector<u_int8_t> &publicKey,
            std::vector<u_int8_t> &privateKey,
            bool needPrivilege = true) const;

    public:
        /**
         * @brief Creates a key and retrieves its ID.
         * @param myId Identifier for the key.
         * @param keyId Reference to store the new key's ID.
         * @param type Encryption algorithm type.
         * @param bits Key size in bits (default: 512).
         * @return Status of the operation as HSM_STATUS.
         */
        HSM_STATUS create_key_and_get_id(
            const Ident &myId,
            KeyId &keyId,
            ENCRYPTION_ALGORITHM_TYPE type,
            int bits = 512);
    };

    /**
     * @class HSM
     * @brief Main class for HSM (Hardware Security Module) operations, including encryption, decryption, signing, and verification.
     *        Inherits from KeyStorage.
     */
    class HSM : public KeyStorage
    {
    private:
        static std::unique_ptr<HSM> instance; ///< Singleton instance of the HSM class.

        /**
         * @brief Private constructor for HSM.
         * @param kekAlgorithmType Encryption algorithm type for the KEK.
         * @param stringKeyForKek Key used for encrypting the KEK.
         */
        explicit HSM(ENCRYPTION_ALGORITHM_TYPE kekAlgorithmType, std::string stringKeyForKek)
            :KeyStorage(kekAlgorithmType, stringKeyForKek) {}

    public:
        /**
         * @brief Friend function to get the singleton instance of HSM.
         * @return Reference to the HSM instance.
         */
        friend HSM &getInstance();

        /**
         * @brief Resets the HSM singleton instance.
         */
        static void resetInstance();

        /**
         * @brief Encrypts a message.
         * @param message The message to encrypt.
         * @param encrypted_message Vector to store the encrypted message.
         * @param type Encryption algorithm type.
         * @param myId Identifier for the key.
         * @param keyId The ID of the key.
         * @param needPrivilege Whether privilege is required for the operation (default: true).
         * @return Status of the operation as HSM_STATUS.
         */
        HSM_STATUS encrypt(
            const std::vector<u_int8_t> &message,
            std::vector<u_int8_t> &encrypted_message,
            ENCRYPTION_ALGORITHM_TYPE type,
            const Ident &myId,
            const KeyId &keyId,
            bool needPrivilege = true) const;

        /**
         * @brief Decrypts a message.
         * @param message The encrypted message.
         * @param decrypted_message Vector to store the decrypted message.
         * @param type Encryption algorithm type.
         * @param myId Identifier for the key.
         * @param keyId The ID of the key.
         * @return Status of the operation as HSM_STATUS.
         */
        HSM_STATUS decrypt(
            const std::vector<u_int8_t> &message,
            std::vector<u_int8_t> &decrypted_message,
            ENCRYPTION_ALGORITHM_TYPE type,
            const Ident &myId,
            const KeyId &keyId) const;

        /**
         * @brief Signs a message.
         * @param message The message to sign.
         * @param signature Vector to store the generated signature.
         * @param sigAlg Signature algorithm type.
         * @param hashAlg Hash algorithm type.
         * @param myId Identifier for the key.
         * @param keyId The ID of the key.
         * @return Status of the operation as HSM_STATUS.
         */
        HSM_STATUS signMessage(
            const std::vector<u_int8_t> &message,
            std::vector<u_int8_t> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            HASH_ALGORITHM_TYPE hashAlg,
            const Ident &myId,
            const KeyId &keyId) const;

        /**
         * @brief Verifies a message signature.
         * @param message The message whose signature is to be verified.
         * @param signature The signature to verify.
         * @param sigAlg Signature algorithm type.
         * @param hashAlg Hash algorithm type.
         * @param myId Identifier for the key.
         * @param keyId The ID of the key.
         * @param needPrivilege Whether privilege is required for the operation (default: true).
         * @return Status of the operation as HSM_STATUS.
         */
        HSM_STATUS verify(
            const std::vector<u_int8_t> &message,
            const std::vector<u_int8_t> &signature,
            ENCRYPTION_ALGORITHM_TYPE sigAlg,
            HASH_ALGORITHM_TYPE hashAlg,
            const Ident &myId,
            const KeyId &keyId,
            bool needPrivilege = true) const;
    };

    /**
     * @brief Gets the singleton instance of HSM.
     * @return Reference to the HSM instance.
     */
    HSM& getInstance();
}
