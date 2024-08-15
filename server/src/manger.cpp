#include "manger.h"
#include "constants.h"
#include <iostream>
#include <string.h>
#include <sstream>


MangServer::MangServer():m_server{PORTSERVER} ,m_req{} , m_connect{}
{
}

void MangServer::init()
{
   
    std::thread t_s (&MangServer::run_server , this);
    std::thread t_select(&MangServer::run_connect, this);

    m_server_socket.create();
    m_server_socket.connect(IPSERVER , PORTSERVER);
    std::stringstream ss;
    ss << 55;
    std::string idStr = ss.str();

    m_server_socket.send((char *)idStr.c_str() ,idStr.size());


   
    t_s.join();
   
    t_select.join();
   
}

void MangServer::run_server()
{
    while (true){
        int fd = m_server.wait_next_client();
       
        if (fd > 0){
              int id = m_connect.add_socket(fd);
              if(id != 55){
                    char buf[4] = "msg";
                    m_server_socket.send(buf, 3);

              }
        }
      
       

    }
}

void MangServer::run_connect()
{
   m_connect.select_menger();
}


