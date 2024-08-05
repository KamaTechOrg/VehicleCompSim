#include "mang_connect.h"
#include <iostream>

std::pair<int, std::shared_ptr<Socket>>  Mange_connect::create(FD fd)
{
    auto new_socket = std::make_shared<Socket>();
    new_socket->set_FD(fd);
    void *data[MAXRECVID];

    new_socket->recv(data, MAXRECVID);
    int i = 0;
    while (*(char *)(data + i) != '\0' )
    {
      std::cout << *(char *)(data + i ) << std::endl;
      i++;
    }
    return std::make_pair(0, new_socket);
}

void Mange_connect::add_to_map(std::pair<int, std::shared_ptr<Socket>> pair)
{
    m_connections[pair.first] = pair.second;
}

std::shared_ptr<Socket> Mange_connect::get_sock(int id)
{
    return m_connections[id];
}
