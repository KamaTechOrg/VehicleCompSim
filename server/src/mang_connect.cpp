#include "mang_connect.h"

std::pair<int,std::shared_ptr<Socket>> Mange_connect::create(int fd)
{
    auto new_socket = std::make_shared<Socket>();
    // new_socket->m_sock = fd;
    // new_socket->recv

    return std::make_pair(0, new_socket);

}

void Mange_connect::add_to_map(std::pair<int,std::shared_ptr<Socket>> pair)
{
    m_connections[pair.first] = pair.second;
}

std::shared_ptr<Socket> Mange_connect::get_sock(int id)
{
    return m_connections[id];
}
