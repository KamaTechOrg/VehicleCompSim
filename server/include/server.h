#pragma once

#include "socket.h"

class Server
{
public:
    Server(int port);
    int run();

private:

    int m_id;
    Socket m_server_socket;
};