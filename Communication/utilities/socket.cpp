
#include <iostream>
#include <cstring>
#include <vector>

#include "socket.h"


Socket::Socket() : m_sock(-1)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        // throw...
    }
#endif
    memset(&m_addr, 0, sizeof(m_addr));
}

Socket::~Socket()
{
    if (is_valid())
    {

#ifdef _WIN32
        WSACleanup();
#else
        close(m_sock);
#endif
    }
}

void Socket::create()
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (!is_valid())
    {
        std::cerr << "Socket creation failed\n";
        // throw...
    }

    int on = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on)) == -1)
    {
        std::cerr << "setsockopt failed\n";
        // throw...
    }
}

void Socket::bind(const int port)
{
    if (!is_valid())
    {
        std::cerr << "Invalid socket\n";
        // throw...
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(port);

    int bind_return = ::bind(m_sock, (struct sockaddr *)&m_addr, sizeof(m_addr));

    if (bind_return == -1)
    {
#ifdef _WIN32
        std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
#else
        std::cout << "bind failed with errno: " << errno << "\n";
#endif
        // throw...
    }
}

void Socket::listen() const
{
    if (!is_valid())
    {
        std::cerr << "Invalid socket\n";
        // throw...
    }

    int listen_return = ::listen(m_sock, SOMAXCONN);

    if (listen_return == -1)
    {
#ifdef _WIN32
        std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
#else
        std::cerr << "listen failed with errno: " << errno << "\n";
#endif
        // throw...
    }
}

FD Socket::accept()
{
    int addr_length = sizeof(m_addr);
    FD fd = ::accept(m_sock, (sockaddr *)&m_addr, (socklen_t *)&addr_length);

    if (fd == -1)
    {
#ifdef _WIN32
        std::cerr << "accept failed with error: " << WSAGetLastError() << "\n";
#else
        std::cerr << "accept failed with errno: " << errno << "\n";
#endif
        // throw...
    }
    else
    {
        std::cout << "Connection established with FD: " << fd << std::endl;
    }
    return fd;
}

sendErrorCode Socket::send(void *data, size_t size) const
{
    sendErrorCode code;
#ifdef _WIN32
    int status = ::send(m_sock, static_cast<const char *>(data), static_cast<int>(size), 0);
#else
    int status = ::send(m_sock, data, size, MSG_NOSIGNAL);
#endif

    if (status == -1)
    {
#ifdef _WIN32
        std::cerr << "send failed with error: " << WSAGetLastError() << "\n";
        code = sendErrorCode::SENDFAILED;
        return code;
#else
        std::cerr << "send failed with errno: " << errno << "\n";
        code = sendErrorCode::SENDFAILED;
        return code;
#endif
      
    }
    code = sendErrorCode::SUCCESS;
        return code;
}

std::pair<ListenErrorCode, int> Socket::recv(void *data, size_t len) const
{
    ListenErrorCode errorCode;
    char buf[len]; 
    memset(buf, 0, len);

#ifdef _WIN32
    int status = ::recv(m_sock, buf, static_cast<int>(len), 0);
#else
    int status = ::recv(m_sock, buf, len, 0);
#endif

    if (status == -1)
    {
#ifdef _WIN32
        std::cerr << "recv failed with error: " << WSAGetLastError() << "\n";
        errorCode = ListenErrorCode::RECEIVE_ERROR;
        return std::make_pair(errorCode, status);
#else
        std::cerr << "recv failed with errno: " << errno << "\n";
        errorCode = ListenErrorCode::RECEIVE_ERROR;
        return std::make_pair(errorCode, status);
#endif
    }
    else if (status == 0)
    {
        errorCode = ListenErrorCode::SERVER_DISCONNECTED;
        return std::make_pair(errorCode, status);
    }
    else
    {
        std::cout << "Received === " << buf << std::endl;

        memcpy(data, buf, len);
    }
    errorCode = ListenErrorCode::SUCCESS;
    return std::make_pair(errorCode, status);
}

void Socket::set_FD(int fd)
{
    m_sock = fd;
}

void Socket::connect(const std::string host, const int port)
{
    if (!is_valid())
    {
        std::cerr << "Invalid socket\n";
        // throw...
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);

    int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);

    status = ::connect(m_sock, (sockaddr *)&m_addr, sizeof(m_addr));

    if (status == -1)
    {
#ifdef _WIN32
        std::cerr << "connect failed with error: " << WSAGetLastError() << "\n";
#else
        std::cerr << "connect failed with errno: " << errno << "\n";
        const char* errorStr = strerror(errno);
        std::cout << errorStr << "\n";

#endif
        // throw...
    }
    else
    {
        std::cout << "Successfully connected to server\n";
    }
}
