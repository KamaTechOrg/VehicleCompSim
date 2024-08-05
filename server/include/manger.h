#pragma once

#include "server.h"
#include "mang_connect.h"
#include "mang_req.h"


#include <thread>
#include <mutex>

class MangServer
{
public:
    MangServer();
    void init();
    void run_server();
    void run_connect();
    void add_socket();
    void change_m_sock();

private:
    Server m_server;
    Mamge_req m_req;
    Mange_connect m_connect;
    FD m_sock_to_eccept = 0;
    std::mutex m_sock_mutex;
};
