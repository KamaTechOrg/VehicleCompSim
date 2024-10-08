#include <cstdint>
#include <string>
#include <vector>

#include "../../HSM_Enums/HSMEnums.hpp"
#include "../../include/HSM.h"

namespace HSMns
{
    class AES
    {
    public:
        static HSM_STATUS encrypt(std::vector<uint8_t> const &clrMsg, std::vector<uint8_t> &encMsg, std::vector<uint8_t> const &key, ENCRYPTION_ALGORITHM_TYPE const &type);
        static HSM_STATUS decrypt(std::vector<uint8_t> const &encMsg, std::vector<uint8_t> &clrMsg, std::vector<uint8_t> const &key,  ENCRYPTION_ALGORITHM_TYPE const &type);

        static HSM_STATUS generateKey(std::vector<uint8_t> &generated_key, ENCRYPTION_ALGORITHM_TYPE const &type);
        static HSM_STATUS generateAndPrintKey(std::vector<uint8_t> &generated_key, ENCRYPTION_ALGORITHM_TYPE const &type);
    };
}
