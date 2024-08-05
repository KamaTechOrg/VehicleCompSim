#include "manger.h"
#include <iostream>
#include "constants.h"


MangServer::MangServer():m_server{PORTSERVER} ,m_req{} , m_connect{}
{
}

void MangServer::init()
{
   
    std::thread t_s (&MangServer::run_server , this);
    std::thread t_change_eccept (&MangServer::change_m_sock , this );
   
    t_s.join();
    t_change_eccept.join();
   
}

void MangServer::run_server()
{
    while (true){
        int fd = m_server.run();
        std::unique_lock<std::mutex> lock(m_sock_mutex);
        m_sock_to_eccept = fd;

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

void MangServer::add_socket()
{
   
    auto pair = m_connect.create(m_sock_to_eccept);
    std::cout << "the fd is : " << m_sock_to_eccept << std::endl;
   
    m_connect.add_to_map(pair);
}

void MangServer::change_m_sock()
{
    while (true){
        std::unique_lock<std::mutex> lock(m_sock_mutex);
        if (m_sock_to_eccept != 0){
            add_socket();
            m_sock_to_eccept = 0;
        }
    }
}
