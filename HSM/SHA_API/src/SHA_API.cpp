#include "SHA_API.h"

HSMns::HSM_STATUS SHA_API::sha(
    const std::vector<u_char> &message, 
    HASH_ALGORITHM_TYPE type
){
    return HSMns::HSM_STATUS();
}

HSMns::HSM_STATUS SHA_API::comperHash(
    const std::vector<u_char> &message, 
    const std::vector<u_char> &hash, 
    HASH_ALGORITHM_TYPE type
){
    return HSMns::HSM_STATUS();
}
