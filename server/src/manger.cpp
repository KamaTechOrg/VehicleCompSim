#include "manger.h"

#include <thread>

MangServer::MangServer():m_server{0} ,m_req{} , m_connect{}
{
}

void MangServer::init()
{
    std::thread t_s (&run_server , this);
    std::thread t_c (&run_connect, this);

    t_s.join();
    t_c.join();
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
   
    // new_socket.msocket = fd;

    m_connect.add_to_map(pair);
}