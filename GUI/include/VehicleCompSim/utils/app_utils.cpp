#include "app_utils.h"
#include <array>
#include <memory>
#include <regex>
#include <stdexcept>

std::string App_Utils::execCommandWithOutput(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe and run the command
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read the output of the command into the buffer
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

bool App_Utils::isValidIP(const std::string& ip) {
    // Regular expression for validating an IPv4 address
    const std::regex ipPattern(
        R"(^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\."
        R"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\."
        R"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\."
        R"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)"
        );

    // Validate IP string using the regex pattern
    return std::regex_match(ip, ipPattern);
}

std::string App_Utils::getPublicIp()
{
    auto output = execCommandWithOutput("curl https://api.ipify.org");
    if (isValidIP(output))
    {
        return output;
    }
    else
    {
        return "";
    }
}


