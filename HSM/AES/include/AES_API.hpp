#include <cstdint>
#include <string>
#include <vector>

#include "../../HsmStatus/HSMStatusEnum.hpp"
using namespace HSM;

namespace HSM
{
    class AES
    {
    public:
        static HSM_STATUS encrypt(std::vector<uint8_t> const &key, std::vector<uint8_t> const &clrMsg, std::vector<uint8_t> &encMsg, std::string const &type = "AES-CBC-256");
        static HSM_STATUS decrypt(std::vector<uint8_t> const &key, std::vector<uint8_t> const &encMsg, std::vector<uint8_t> &clrMsg, std::string const &type = "AES-CBC-256");

        static HSM_STATUS generateKey(std::vector<uint8_t> &generated_key, std::string const &type = "AES-CBC-256");
    };
}
