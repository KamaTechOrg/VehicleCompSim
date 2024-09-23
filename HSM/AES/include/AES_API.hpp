#include <cstdint>
#include <string>
#include <vector>
class AES {
public:
    static std::vector<uint8_t> encrypt(std::vector<uint8_t> const& key, std::vector<uint8_t> const& message, std::string const& type = "AES-CBC-256");
    static std::vector<uint8_t> decrypt(std::vector<uint8_t> const& key, std::vector<uint8_t> const& message, std::string const& type = "AES-CBC-256");
};
