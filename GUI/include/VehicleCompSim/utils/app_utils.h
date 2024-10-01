#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <string>

namespace App_Utils{
std::string execCommandWithOutput(const char* cmd);
bool isValidIP(const std::string& ip);
std::string getPublicIp();
}

#endif // APP_UTILS_H
