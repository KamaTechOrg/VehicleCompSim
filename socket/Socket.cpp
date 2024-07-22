#include "Socket.h"

Socket::Socket() : sock(INVALID_SOCKET) {
    if (!initialize()) {
        std::cerr << "Failed to initialize socket." << std::endl;
    }
}

Socket::~Socket() {
    close();
    cleanup();
}

bool Socket::initialize() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}

void Socket::cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

bool Socket::connect(const std::string& host, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());

    if (::connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server." << std::endl;
        close();
        return false;
    }

    return true;
}

void Socket::close() {
    if (sock != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(sock);
#else
        ::close(sock);
#endif
        sock = INVALID_SOCKET;
    }
}

int Socket::send(const std::string& data) {
    return ::send(sock, data.c_str(), data.length(), 0);
}

int Socket::receive(char* buffer, int size) {
    return ::recv(sock, buffer, size, 0);
}
