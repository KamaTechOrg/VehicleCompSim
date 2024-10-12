#include <cstdint>
#include <string>
#include <vector>

#include "../../HSM_Enums/HSMEnums.hpp"
#include "HSM.h"

namespace HSMns
{
    /**
     * @class AES
     * @brief Provides AES encryption and decryption functionality.
     */
    class AES
    {
    public:
        /**
         * @brief Encrypts a clear message using the specified AES encryption algorithm.
         * 
         * @param clrMsg The clear message to encrypt, represented as a vector of bytes.
         * @param encMsg The resulting encrypted message, represented as a vector of bytes.
         * @param key The encryption key to use, represented as a vector of bytes.
         * @param type The type of AES encryption algorithm to use.
         * 
         * @return HSM_STATUS Status code indicating the success or failure of the encryption process.
         */
        static HSM_STATUS encrypt(std::vector<uint8_t> const &clrMsg, std::vector<uint8_t> &encMsg, std::vector<uint8_t> const &key, ENCRYPTION_ALGORITHM_TYPE const &type);

        /**
         * @brief Decrypts an encrypted message using the specified AES decryption algorithm.
         * 
         * @param encMsg The encrypted message to decrypt, represented as a vector of bytes.
         * @param clrMsg The resulting clear message after decryption, represented as a vector of bytes.
         * @param key The decryption key to use, represented as a vector of bytes.
         * @param type The type of AES decryption algorithm to use.
         * 
         * @return HSM_STATUS Status code indicating the success or failure of the decryption process.
         */
        static HSM_STATUS decrypt(std::vector<uint8_t> const &encMsg, std::vector<uint8_t> &clrMsg, std::vector<uint8_t> const &key,  ENCRYPTION_ALGORITHM_TYPE const &type);

        /**
         * @brief Generates a random AES key for the specified encryption algorithm type.
         * 
         * @param generated_key The generated AES key, represented as a vector of bytes.
         * @param type The type of AES encryption algorithm for which to generate the key.
         * 
         * @return HSM_STATUS Status code indicating the success or failure of the key generation process.
         */
        static HSM_STATUS generateKey(std::vector<uint8_t> &generated_key, ENCRYPTION_ALGORITHM_TYPE const &type);

        /**
         * @brief Generates a random AES key for the specified encryption algorithm type and prints it.
         * 
         * @param generated_key The generated AES key, represented as a vector of bytes.
         * @param type The type of AES encryption algorithm for which to generate the key.
         * 
         * @return HSM_STATUS Status code indicating the success or failure of the key generation and printing process.
         */
        static HSM_STATUS generateAndPrintKey(std::vector<uint8_t> &generated_key, ENCRYPTION_ALGORITHM_TYPE const &type);
    };
}
