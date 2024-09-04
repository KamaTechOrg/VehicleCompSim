#pragma once

#include "server.h"
#include "receive_manger.h"
#include "mang_req.h"
#include "canbus.h"


#include <queue>
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
    void init_inner();
    
   

    Server m_server;
    Mamge_req m_req;
    Receive_manger m_connect;
    Socket m_server_socket;
    std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> m_min_heap;
  
};
