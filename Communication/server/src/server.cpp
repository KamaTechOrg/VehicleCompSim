#include "server.h"

Server::Server(int port)
{
    m_server_socket.create();
    m_server_socket.bind(port);
    m_server_socket.listen();
}

FD Server::wait_next_client()
{
    return m_server_socket.accept();
}
