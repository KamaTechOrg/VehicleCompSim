#ifndef SOCKET_HPP
#define SOCKET_HPP

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cstring>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
#endif

#include <iostream>
#include <string>

class Socket {
public:
    Socket();
    ~Socket();
    bool connect(const std::string& host, int port);
    void close();
    int send(const std::string& data);
    int receive(char* buffer, int size);

private:
    SOCKET sock;
    bool initialize();
    void cleanup();
};

#endif // SOCKET_HPP
