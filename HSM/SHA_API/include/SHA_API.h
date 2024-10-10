#pragma once

#ifndef SHA_API_H
#define SHA_API_H

#include "HSMEnums.hpp"
#include <vector>
#include <string>
#include <cstdint>

/**
 * @enum HASH_ALGORITHM_TYPE
 * @brief Enum representing different hash algorithm types.
 */
enum HASH_ALGORITHM_TYPE
{
    NoHash,       /**< No hashing */
    SHA1,         /**< SHA-1 hash algorithm */
    SHA256,       /**< SHA-256 hash algorithm */
    SHA512,       /**< SHA-512 hash algorithm */
    Hash_Count,   /**< Count of hash types */
    SHA3_256,     /**< SHA3-256 hash algorithm */
    SHA3_512,     /**< SHA3-512 hash algorithm */
};

/**
 * @class SHA_API
 * @brief Class providing hashing-related functionality.
 */
class SHA_API
{
public:
    /**
     * @brief Generates a hash of the provided message using the specified hash algorithm.
     * 
     * @param message The message to hash as a vector of unsigned chars.
     * @param type The hash algorithm to use (default is SHA256).
     * 
     * @return HSMns::HSM_STATUS Status code indicating the result of the operation.
     */
    static HSMns::HSM_STATUS sha(
        const std::vector<u_char> &message, 
        HASH_ALGORITHM_TYPE type = HASH_ALGORITHM_TYPE::SHA256
    );

    /**
     * @brief Compares a message with a provided hash using the specified hash algorithm.
     * 
     * @param message The message to hash and compare as a vector of unsigned chars.
     * @param hash The precomputed hash to compare against.
     * @param type The hash algorithm to use for comparison (default is SHA256).
     * 
     * @return HSMns::HSM_STATUS Status code indicating whether the comparison was successful.
     */
    static HSMns::HSM_STATUS compareHash(
        const std::vector<u_char> &message, 
        const std::vector<u_char> &hash, 
        HASH_ALGORITHM_TYPE type = HASH_ALGORITHM_TYPE::SHA256
    );
};

#endif
