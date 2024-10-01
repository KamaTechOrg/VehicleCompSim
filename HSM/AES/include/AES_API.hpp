#include <cstdint>
#include <string>
#include <vector>

#include "../../HSM_Enums/HSMEnums.hpp"
#include "../../include/HSM.h"
using namespace HSM;

namespace HSM
{
    class AES
    {
    public:
        static HSM_STATUS encrypt(std::vector<uint8_t> const &key, std::vector<uint8_t> const &clrMsg, std::vector<uint8_t> &encMsg, ENCRYPTION_ALGORITHM_TYPE const &type);
        static HSM_STATUS decrypt(std::vector<uint8_t> const &key, std::vector<uint8_t> const &encMsg, std::vector<uint8_t> &clrMsg, ENCRYPTION_ALGORITHM_TYPE const &type);

        static HSM_STATUS generateKey(std::vector<uint8_t> &generated_key, ENCRYPTION_ALGORITHM_TYPE const &type);
    };
}
