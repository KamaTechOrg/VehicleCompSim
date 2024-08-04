#include "server.h"

Server::Server(int port)
{
    m_server_socket.create();
    m_server_socket.bind(port);
    m_server_socket.listen();
}

int Server::run()
{
    return m_server_socket.accept();
}
