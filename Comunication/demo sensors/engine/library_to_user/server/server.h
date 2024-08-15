#pragma once

#include "../../../../demo sensors/engine/library_to_user/utilities/socket.h"

class Server
{
public:
    Server(int port);
    FD wait_next_client();

private:

    int m_id;
    Socket m_server_socket;
};