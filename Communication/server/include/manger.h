#pragma once

#include <thread>

#include "server.h"
#include "receive_manger.h"
#include "send_menger.h"

class MangServer
{
public:
    MangServer();
    void init();
    
    std::unordered_map<int, FD> m_connections;

private:
    void run_server();
    void run_connect();
    void run_sender();
    void init_inner();
    int add_socket(int new_socket);
    std::pair<int, FD> create_kay_value_id(int fd);
    FD get_sock(int id);


    Server m_server;
    Socket m_server_socket;
    Receive_manger m_connect;
    Send_manager m_send_manager;
    std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> m_min_heap;
    std::mutex m_heap_mutex;
    std::mutex m_map_mutex;
};
