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

private:

    void run_server();
    void run_connect();
    void insert_socket();

    Server m_server;
    Mamge_req m_req;
    Receive_manger m_connect;
    FD new_fd = 0;
    std::mutex new_fd_mutex;
};
