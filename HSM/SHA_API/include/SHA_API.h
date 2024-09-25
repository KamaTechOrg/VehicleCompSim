#pragma once

#ifndef SHA_API_H
#define SHA_API_H

#include "HSMStatusEnum.hpp"
#include <vector>
#include <string>
#include <cstdint>

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
class SHA_API
{
public:
    static HSM::HSM_STATUS sha(
        const std::vector<u_char> &message, 
        HASH_ALGORITHM_TYPE type = HASH_ALGORITHM_TYPE::SHA256
    );

    static HSM::HSM_STATUS comperHash(
        const std::vector<u_char> &message, 
        const std::vector<u_char> &hash, 
        HASH_ALGORITHM_TYPE type = HASH_ALGORITHM_TYPE::SHA256
    );
};


#endif