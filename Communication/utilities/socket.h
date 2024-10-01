#pragma once

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET FD;  // Use SOCKET as the type for file descriptors in Windows
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    typedef int FD;  // Use int as the type for file descriptors in Linux
#endif


#include "canbus.h"
#include "constants.h"
#include "data_manipulator.h"

class Socket
{
public:
    Socket();
    ~Socket();

    void create();
    void bind(const int port);
    void listen() const;
    FD accept();

    void connect(const std::string host, const int port);

    sendErrorCode send(void *data, size_t size) const;
    std::pair<ListenErrorCode,int> recv(void *data, size_t len) const;

    void set_FD(int fd);
    int get_FD() const { return m_sock; }

    bool is_valid() const { return m_sock != -1; }
    void close();

private:
    FD m_sock;  
    sockaddr_in m_addr;
};

