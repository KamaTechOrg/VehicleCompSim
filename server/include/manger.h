#pragma once

#include <thread>


#include "server.h"
#include "mang_req.h"
#include "receive_manger.h"
#include "send_menger.h"




class MangServer
{
public:
    MangServer();
    void init();

 

private:

    void run_server();
    void run_connect();
    void run_sender();
    void init_inner();
    
   

    Server m_server;
    Mamge_req m_req;
    Socket m_server_socket;
    Receive_manger m_connect;
    Send_manager m_send_manager;
    std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> m_min_heap;
    std::mutex m_heap_mutex;  
  
};
