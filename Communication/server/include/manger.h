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
    
    // get_map();
    std::unordered_map<int, FD> m_connections;

private:
    void run_server();
    void run_connect();
    void run_sender();
    void init_inner();
    int add_socket(int new_socket);
    std::pair<int, FD> create_kay_value_id(int fd);
    FD get_sock(int id);


    Server m_server_eccept_socket;
    // 
    Socket m_controll_socket;
    Receive_manger m_recv_manger;
    Send_manager m_send_manager;
    std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> m_prioritysed_masseges_queue;
    std::mutex m_queue_mutex;
    std::mutex m_map_mutex;
};
