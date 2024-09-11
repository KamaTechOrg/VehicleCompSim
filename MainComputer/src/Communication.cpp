#include "Communication.h"
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QDebug>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#include "SensorsManager.h"

Communication::Communication() {
    initializeWinsock();
}

Communication::~Communication() {
    cleanupWinsock();
}

void Communication::initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        qWarning() << "WSAStartup failed: " << result;
        exit(EXIT_FAILURE);
    }
}

void Communication::cleanupWinsock() {
    WSACleanup();
}

int Communication::createSocket() {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        qWarning() << "Could not create socket: " << WSAGetLastError();
        exit(EXIT_FAILURE);
    }
    return sock;
}

std::string Communication::listenTo(int portNumber) {
    int serverSock = createSocket();
    struct sockaddr_in serverAddr;
    char buffer[1024] = { 0 };

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portNumber);

    // Bind
    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        qWarning() << "Bind failed: " << WSAGetLastError();
        closesocket(serverSock);
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
        qWarning() << "Accept failed: " << WSAGetLastError();
        closesocket(serverSock);
        return "ERROR";
    }

    // Receive data
    int valread = recv(clientSock, buffer, sizeof(buffer), 0);
    if (valread == SOCKET_ERROR) {
        qWarning() << "Receive failed: " << WSAGetLastError();
    }

    closesocket(clientSock);
    closesocket(serverSock);

    return std::string(buffer);
}

void Communication::sendTo(int portNumber, const std::string& message) {
    int clientSock = createSocket();
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNumber);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        qWarning() << "Invalid address/ Address not supported";
        closesocket(clientSock);
        return;
    }

    // Connect to the server
    if (connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        qWarning() << "Connection Failed: " << WSAGetLastError();
        closesocket(clientSock);
        return;
    }

    // Send the message
    send(clientSock, message.c_str(), message.length(), 0);

    closesocket(clientSock);
}

void Communication::connectToSensors()
{
    std::vector<int> sensorsPortNumbers = SensorsManager().getPortNumbers();
    for (const int portNumber : sensorsPortNumbers) {
        int sensorSock = createSocket();
        struct sockaddr_in sensorAddr;

        sensorAddr.sin_family = AF_INET;
        sensorAddr.sin_port = htons(portNumber);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &sensorAddr.sin_addr) <= 0) {
            qWarning() << "Invalid address/ Address not supported";
            closesocket(sensorSock);
            return;
        }

        // Connect to the server
        if (connect(sensorSock, (struct sockaddr*)&sensorAddr, sizeof(sensorAddr)) < 0) {
            qWarning() << "Connection Failed: " << WSAGetLastError();
            closesocket(sensorSock);
            return;
        }

        // Continuously receive messages from the server
        std::thread([this, sensorSock]() {

            // Send initial message
            std::string initMessage = "INIT";
            send(sensorSock, initMessage.c_str(), initMessage.length(), 0);

            while (true) {
                char buffer[1024] = { 0 };
                int valread = recv(sensorSock, buffer, sizeof(buffer), 0);
                if (valread > 0) {
                    qInfo() << "recived: " << buffer;

                    std::lock_guard<std::mutex> lock(queueMutex);
                    _messagesQueue.push(std::string(buffer, valread));
                    messageAvailable.notify_one(); // Signal new message arrival
                }
                else {
                    break;
                }
            }
            closesocket(sensorSock);
            }).detach();
    }
}

std::string Communication::getMessageFromQueue()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    messageAvailable.wait(lock, [this] { return !_messagesQueue.empty(); }); // Wait for a message
    std::string message = _messagesQueue.front();
    _messagesQueue.pop();
    return message;
}
