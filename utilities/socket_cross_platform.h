#pragma once
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET FD; // Use SOCKET as the type for file descriptors in Windows
#else
#include <unistd.h>
#endif

#include "constants.h"
#include "socket.h"

namespace Cross_platform
{
    int cress_read(int sd, char *buf, int flags);
    int cress_send(FD d_s, const char *buf, size_t size);
}
