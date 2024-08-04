#include "manger.h"

#include <thread>

MangServer::MangServer():m_server{50000} ,m_req{} , m_connect{}
{
}

void MangServer::init()
{
     auto callback = [this](int fd) {
        this->add_socket(fd);
    };
    std::thread t_s (&MangServer::run_server , this, callback );
    // std::thread t_c (&run_connect, this);
   
    t_s.join();
    // t_c.join();
}

void MangServer::run_server(std::function<void(int)> callback)
{
    while (true){
        int fd = m_server.run();
        callback(fd);
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

void MangServer::add_socket(int FD)
{
    auto pair = m_connect.create(FD);
   
    m_connect.add_to_map(pair);
}