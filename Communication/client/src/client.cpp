
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <future>
#include <thread>

#include "client.h"

static void send_yuor_id(int id , ClientSocket &socket)
{
    std::string idStr = Data_manipulator::int_to_str(id);

    socket.send((char *)idStr.c_str(), idStr.size(), id, id);
    char buffer[20];
    socket.listen(buffer, sizeof(buffer));
    if (strcmp(buffer, "OK") != 0)
    {
        throw std::runtime_error(buffer);
    }
}

ClientSocket::ClientSocket(int id)
    : my_id{id}, m_clientSocket{std::make_shared<Socket>()}

{
    if (id <= 0)
    {
        throw std::invalid_argument("Invalid ID: must be a positive integer.");
    }
    
    m_clientSocket->create();
    std::string ip_server = Data_manipulator::get_ip_server(IPFILENAME);
    m_clientSocket->connect(ip_server, PORTSERVER);
    
    send_yuor_id(id, *this);
}

ClientSocket::~ClientSocket()
{
    m_clientSocket->close();
}

sendErrorCode ClientSocket::send(void *data, size_t size, int source_id, int dest_id)
{
    if (!is_valid_ptr(data) || !is_valid_size(size) || !is_valid_d_id(dest_id))
    {
        throw std::runtime_error("Invalid input");
    }

    sendErrorCode code;
    auto pair_data = Data_manipulator::data_and_id_to_str(data, size, source_id, dest_id);
    auto clientSocket = m_clientSocket;
    if (clientSocket)
    {
        return code = clientSocket->send((void *)pair_data.first.c_str(), pair_data.second);
    }
    code = sendErrorCode::SENDFAILED;
    return code;
    
}

std::pair<ListenErrorCode, int> ClientSocket::listen(void *data, size_t size)
{
    auto clientSocket = m_clientSocket;
    if (!is_valid_ptr(data) || !is_valid_size(size))
    {
        throw std::runtime_error("Invalid to receive");
    }
    std::pair<ListenErrorCode, int> pair_recv;
    if (clientSocket)
    {
        pair_recv = clientSocket->recv(data, size);
    }

    std::cout << "RECV: " << pair_recv.first << std::endl;
    return pair_recv;
}

void ClientSocket::listenAsync(void *data, size_t size, std::function<void(ListenErrorCode)> callback)
{
    if (!is_valid_ptr(data) || !is_valid_size(size))
    {
        throw std::runtime_error("Invalid to receive");
    }
    auto clientSocket = m_clientSocket;
    if (clientSocket)
    {
        std::thread t1([&clientSocket, this, data, size, callback]()
                       {                 
            auto pair_recv = clientSocket->recv(data, size);
            if (callback)
            {
                callback(pair_recv.first);
            } });

        t1.detach();
    }
}

bool ClientSocket::is_valid_ptr(void *ptr)
{
    return ptr;
}

bool ClientSocket::is_valid_size(size_t size)
{
    return size <= MAXRECV;
}

void ClientSocket::shut_down()
{
    m_clientSocket =nullptr;
}

bool ClientSocket::is_valid_d_id(int d_id)
{
    return d_id > 0;
}