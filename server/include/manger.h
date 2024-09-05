#pragma once


#include <queue>
#include <thread>
#include <mutex>

#include "server.h"
#include "mang_req.h"
#include "receive_manger.h"




class MangServer
{
public:
    MangServer();
    void init();
    void sender();

 

private:

    void run_server();
    void run_connect();
    void init_inner();
    
   

    Server m_server;
    Mamge_req m_req;
    Receive_manger m_connect;
    Socket m_server_socket;
    std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> m_min_heap;
    std::mutex m_heap_mutex;  
  
};
