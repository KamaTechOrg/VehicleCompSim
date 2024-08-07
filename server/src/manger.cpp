#include "manger.h"
#include <iostream>
#include "constants.h"


MangServer::MangServer():m_server{PORTSERVER} ,m_req{} , m_connect{}
{
}

void MangServer::init()
{
   
    std::thread t_s (&MangServer::run_server , this);
    std::thread t_new_connection (&MangServer::insert_socket , this );
    std::thread t_select(&MangServer::run_connect, this);
   
    t_s.join();
    t_new_connection.join();
    t_select.join();
   
}

void MangServer::run_server()
{
    while (true){
        int fd = m_server.wait_next_client();
        std::unique_lock<std::mutex> lock(new_fd_mutex);
        new_fd = fd;

    }
}

void MangServer::run_connect()
{
   m_connect.select_menger();
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
