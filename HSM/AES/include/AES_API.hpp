#include <string>
class AES {
public:
    static std::string encrypt(std::string const& type, std::string const& key, std::string const& message);
    static std::string decrypt(std::string const& type, std::string const& key, std::string const& message);
};
