
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
#include <algorithm>

#include "client.h"


ClientSocket::ClientSocket(int id)
    : my_id(id)
{
    m_clientSocket.create();
    m_clientSocket.connect(IPSERVER, PORTSERVER);

    std::string idStr = Data_manipulator::int_to_str(id);

    m_clientSocket.send((char *)idStr.c_str(), idStr.size());
    char buffer[20];
    listen(buffer, sizeof(buffer));
    if (strcmp(buffer, "OK") != 0)
    {
        throw std::runtime_error(buffer);
    }
}

void ClientSocket::send(void *data, size_t size, int source_id, int dest_id)
{
    if (!is_valid_ptr(data) || !is_valid_size(size))
    {
        throw std::runtime_error("Invalid input");
    }

    std::string id_data = Data_manipulator::data_and_id_to_str(data ,size ,source_id ,dest_id);
    size_t total_size = id_data.size();

    m_clientSocket.send((void *)id_data.c_str(), total_size);
}

void ClientSocket::listen(void *data, size_t size)
{
    if (!is_valid_ptr(data) || !is_valid_size(size))
    {
        throw std::runtime_error("Invalid to receive");
    }
    int len = m_clientSocket.recv(data, size);
}

bool ClientSocket::is_valid_ptr(void *ptr)
{
    return ptr;
}

bool ClientSocket::is_valid_size(size_t size)
{
    return size <= MAXRECV;
}
