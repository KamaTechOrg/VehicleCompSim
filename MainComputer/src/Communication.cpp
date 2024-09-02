#include "Communication.h"
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

void Communication::initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Communication::cleanupWinsock() {
    WSACleanup();
}

int Communication::createSocket() {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Could not create socket: " << WSAGetLastError() << std::endl;
        cleanupWinsock();
        exit(EXIT_FAILURE);
    }
    return sock;
}

std::string Communication::listenTo(int portNumber) {
    initializeWinsock();
    int serverSock = createSocket();
    struct sockaddr_in serverAddr;
    char buffer[1024] = { 0 };

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portNumber);

    // Bind
    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        cleanupWinsock();
        return "ERROR";
    }

    // Listen
    listen(serverSock, 3);

    // Accept an incoming connection
    int clientSock;
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        cleanupWinsock();
        return "ERROR";
    }

    // Receive data
    int valread = recv(clientSock, buffer, sizeof(buffer), 0);
    if (valread == SOCKET_ERROR) {
        std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
    }

    closesocket(clientSock);
    closesocket(serverSock);
    cleanupWinsock();

    return std::string(buffer);
}

void Communication::sendTo(int portNumber, const std::string& message) {
    initializeWinsock();
    int clientSock = createSocket();
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNumber);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        closesocket(clientSock);
        cleanupWinsock();
        return;
    }

    // Connect to the server
    if (connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection Failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSock);
        cleanupWinsock();
        return;
    }

    // Send the message
    send(clientSock, message.c_str(), message.length(), 0);

    closesocket(clientSock);
    cleanupWinsock();
}
