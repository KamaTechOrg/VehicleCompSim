#include <iostream>
#include <sstream>
#include <algorithm>
#include "HSM.h"

#ifdef _WIN32
#include <windows.h>
#include <lmcons.h>
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

using namespace HSMnamespace;

Ident::Ident()
{
    std::string username;
#ifdef _WIN32
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    if (GetUserNameA(username, &username_len))
    {
        username = username;
    }
#elif __linux__
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw)
    {
        username = pw->pw_name;
    }
    else
    {
        std::cerr << "Failed to get user information" << std::endl;
    }
#endif
    // remove "," from username
    std::replace(username.begin(), username.end(), ',', ' ');
    this->id = std::vector<u_int8_t>(username.begin(), username.end());
}

Ident::Ident(const std::string &userID)
{
    this->id = std::vector<u_int8_t>(userID.begin(), userID.end());
}

HSM_STATUS Ident::compareID(const Ident &other) const 
{
    if (this->id.size() != other.id.size())
    {
        return HSM_STATUS::HSM_InternalErr;
    }
    for (int i = 0; i < this->id.size(); i++)
    {
        if (this->id[i] != other.id[i])
        {
            return HSM_STATUS::HSM_InternalErr;
        }
    }
    return HSM_STATUS::HSM_Good;
}

std::string Ident::toString() const
{
    std::string str(this->id.begin(), this->id.end());
    return str;
}
