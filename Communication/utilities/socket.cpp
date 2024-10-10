

#include "socket.h"
#include "Logger.h"

Socket::Socket() : m_sock(-1)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        LOG_WARN("WSAStartup failed");
    }
#endif
    memset(&m_addr, 0, sizeof(m_addr));
}

Socket::~Socket()
{
    close();
}

void Socket::create()
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (!is_valid())
    {
        LOG_WARN("Socket creation failed");
    }

    int on = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on)) == -1)
    {
        LOG_WARN("setsockopt failed");
    }
}

void Socket::bind(const int port)
{
    if (!is_valid())
    {
        LOG_WARN("Invalid socket");
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(port);

    int bind_return = ::bind(m_sock, (struct sockaddr *)&m_addr, sizeof(m_addr));

    if (bind_return == -1)
    {
#ifdef _WIN32
        std::stringstream ss;
        ss << "bind failed with errno: " << WSAGetLastError();
        LOG_ERROR(ss.str());
#else
        std::stringstream ss;
        ss << "bind failed with errno: " << errno;
        LOG_ERROR(ss.str());
#endif
    }
}

void Socket::listen() const
{
    if (!is_valid())
    {
        LOG_WARN("Invalid socket");
    }

    int listen_return = ::listen(m_sock, SOMAXCONN);

    if (listen_return == -1)
    {
#ifdef _WIN32
        std::stringstream ss;
        ss << "listen failed with errno: " << WSAGetLastError();
        LOG_ERROR(ss.str());
#else
        std::stringstream ss;
        ss << "listen failed with errno: " << errno;
        LOG_ERROR(ss.str());
#endif
    }
}

FD Socket::accept()
{
    int addr_length = sizeof(m_addr);
    FD fd = ::accept(m_sock, (sockaddr *)&m_addr, (socklen_t *)&addr_length);

    if (fd == -1)
    {
#ifdef _WIN32
        std::stringstream ss;
        ss << "accept failed with errno: " << WSAGetLastError();
        LOG_ERROR(ss.str());
#else
        std::stringstream ss;
        ss << "accept failed with errno: " << errno;
        LOG_ERROR(ss.str());
#endif
    }
    else
    {
        LOG_INFO("New connection established");
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
    std::cout << status << std::endl;
#endif

    if (status == -1)
    {
#ifdef _WIN32
        std::stringstream ss;
        ss << "send failed with errno: " << WSAGetLastError();
        LOG_ERROR(ss.str());
        code = sendErrorCode::SENDFAILED;
        return code;
#else
        std::stringstream ss;
        ss << "send failed with errno: " << errno;
        LOG_ERROR(ss.str());
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
    char buf[MAXRECV];
    memset(buf, 0, len);

#ifdef _WIN32
    int status = ::recv(m_sock, buf, static_cast<int>(len), 0);
#else
    int status = ::recv(m_sock, buf, len, 0);
#endif

    if (status == -1)
    {
#ifdef _WIN32
        std::stringstream ss;
        ss << "recv failed with errno: " << WSAGetLastError();
        LOG_ERROR(ss.str());
        errorCode = ListenErrorCode::RECEIVE_ERROR;
        return std::make_pair(errorCode, status);

#else
        std::stringstream ss;
        ss << "recv failed with errno: " << errno;
        LOG_ERROR(ss.str());
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
        memcpy(data, buf, len);
        LOG_INFO(buf);

    }
    errorCode = ListenErrorCode::SUCCESS;
    return std::make_pair(errorCode, status);
}

void Socket::set_FD(int fd)
{
    m_sock = fd;
}

void Socket::close()
{
    if (is_valid())
    {

#ifdef _WIN32
        WSACleanup();
#else
        ::close(m_sock);
#endif
    }
}

void Socket::connect(const std::string host, const int port)
{
    if (!is_valid())
    {
        LOG_ERROR("Invalid socket");
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);

    int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);

    status = ::connect(m_sock, (sockaddr *)&m_addr, sizeof(m_addr));

    if (status == -1)
    {
#ifdef _WIN32
        std::stringstream ss;
        ss << "connect failed with error: " << WSAGetLastError();
        LOG_ERROR(ss.str());
#else
        std::stringstream ss;
        ss << "connect failed with errno: " << errno;
        LOG_ERROR(ss.str());
#endif
    }
    else
    {
        LOG_INFO("Successfully connected to server");
    }
}
