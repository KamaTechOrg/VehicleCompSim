#pragma once

#include "server.h"
#include "mang_connect.h"
#include "mang_req.h"

#include <functional>

class MangServer
{
public:
    MangServer();
    void init();
    void run_server(std::function<void(int)> callback);
    void run_connect();
    void add_socket(int FD);

private:
    Server m_server;
    Mamge_req m_req;
    Mange_connect m_connect;
};
