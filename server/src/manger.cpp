#include <chrono>    


#include "manger.h"


MangServer::MangServer() : m_server{PORTSERVER}, m_req{}, m_connect{},m_send_manager{}
{
}

void MangServer::init()
{

    std::thread t_s(&MangServer::run_server, this);
    std::thread t_select(&MangServer::run_connect, this);
    std::thread t_sender(&MangServer::run_sender, this);
    init_inner();
    
    t_s.join();
    t_select.join();
    t_sender.join();
}

void MangServer::run_server()
{   


     while (true)
    {
        int fd = m_server.wait_next_client();

        if (fd > 0)
        {

            int id = m_connect.add_socket(fd);
            if (id != IDINNER)
            {
                char buf[10] = "msg";
                m_server_socket.send(buf, sizeof(buf));

                m_connect.m_condition.notify_one();
            }
        }
    }
}

void MangServer::run_connect()
{
    m_connect.select_menger(m_min_heap , m_heap_mutex);
    
}

void MangServer::init_inner()
{
    m_server_socket.create();
    m_server_socket.connect(IPSERVER, PORTSERVER);

    std::string idStr = Data_manipulator::int_to_str(IDINNER);

    m_server_socket.send((char *)idStr.c_str(), idStr.size());
}


void MangServer::run_sender() {
    auto get_sock_func = [this](int id) { return m_connect.get_sock(id); };
    
    while (true) {

        m_send_manager(m_min_heap,m_heap_mutex,m_connect.m_map_mutex ,get_sock_func);
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}