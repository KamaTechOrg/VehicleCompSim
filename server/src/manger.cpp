#include "manger.h"
#include <iostream>
#include "constants.h"


MangServer::MangServer():m_server{PORTSERVER} ,m_req{} , m_connect{}
{
}

void MangServer::init()
{
   
    std::thread t_s (&MangServer::run_server , this);
    std::thread t_change_eccept (&MangServer::insert_socket , this );
   
    t_s.join();
    t_change_eccept.join();
   
}

void MangServer::run_server()
{
    while (true){
        int fd = m_server.run();
        std::unique_lock<std::mutex> lock(new_fd_mutex);
        new_fd = fd;

    }
}

void MangServer::run_connect()
{
    while(true){
        m_req.recv();
        m_req.send();
        // TODO: use selct to recv and send packets
    }
}


void MangServer::insert_socket()
{
    while (true){
        std::unique_lock<std::mutex> lock(new_fd_mutex);
        if (new_fd != 0){
            m_connect.add_socket(new_fd);
            new_fd = 0;
        }
    }
}
