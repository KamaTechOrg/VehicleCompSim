
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <future>
#include <thread>

#include "client.h"

ClientSocket::ClientSocket(int id)
    : my_id{id}, m_clientSocket{std::make_shared<Socket>()}

{

    if (id <= 0)
    {
        throw std::invalid_argument("Invalid ID: must be a positive integer.");
    }

    m_clientSocket->create();
    m_clientSocket->connect(IPSERVER, PORTSERVER);

    std::string idStr = Data_manipulator::int_to_str(id);

    m_clientSocket->send((char *)idStr.c_str(), idStr.size());
    char buffer[20];
    listen(buffer, sizeof(buffer));
    if (strcmp(buffer, "OK") != 0)
    {
        throw std::runtime_error(buffer);
    }
}

sendErrorCode ClientSocket::send(void *data, size_t size, int source_id, int dest_id)
{
    sendErrorCode code;
    if (!is_valid_ptr(data) || !is_valid_size(size) || !is_valid_d_id(dest_id))
    {
        throw std::runtime_error("Invalid input");
    }

    std::string all_data = Data_manipulator::data_and_id_to_str(data, size, source_id, dest_id);
    size_t total_size = all_data.size();
    if (m_clientSocket)
    {
       return code = m_clientSocket->send((void *)all_data.c_str(), total_size);
    }
    else{
        code = sendErrorCode::SENDFAILED;
        return code;
    }

}

std::pair<ListenErrorCode, int> ClientSocket::listen(void *data, size_t size)
{
    auto clientSocket = m_clientSocket;
    if (!is_valid_ptr(data) || !is_valid_size(size))
    {
        throw std::runtime_error("Invalid to receive");
    }
    auto pair_recv = clientSocket->recv(data, size);

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
    std::thread t1([&clientSocket ,this, data, size, callback]()
                   {                 
            auto pair_recv = clientSocket->recv(data, size);
            if (callback)
            {
                callback(pair_recv.first);
            } });

    t1.detach();
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
    m_clientSocket = nullptr;
}

bool ClientSocket::is_valid_d_id(int d_id)
{
    return d_id > 0;
}