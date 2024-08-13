#include "manger.h"
#include <iostream>
#include "constants.h"


MangServer::MangServer():m_server{PORTSERVER} ,m_req{} , m_connect{}
{
}

void MangServer::init()
{
   
    std::thread t_s (&MangServer::run_server , this);
    std::thread t_select(&MangServer::run_connect, this);
   
    t_s.join();
   
    t_select.join();
   
}

void MangServer::run_server()
{
    while (true){
        int fd = m_server.wait_next_client();
       
        if (fd > 0){
              m_connect.add_socket(fd);
        }
      
       

    }
}

void MangServer::run_connect()
{
   m_connect.select_menger();
}


