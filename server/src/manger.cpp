#include <iostream>
#include <string.h>
#include <sstream>
#include <cstring> 
#include <chrono>    
#include <thread> 

#include "manger.h"


MangServer::MangServer() : m_server{PORTSERVER}, m_req{}, m_connect{}
{
}

void MangServer::init()
{

    std::thread t_s(&MangServer::run_server, this);
    std::thread t_select(&MangServer::run_connect, this);
    std::thread t_sender(&MangServer::sender, this);
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


void MangServer::sender() {
    while (true) {
        std::unique_lock<std::mutex> lock(m_heap_mutex);

        while (!m_min_heap.empty()) {
            
            
            CanBus topElement = m_min_heap.top();

            std::unique_lock<std::mutex> lock(m_connect.m_map_mutex);
            auto d_s = m_connect.get_sock(topElement.getDestinationId());
            lock.unlock();
            char dataCopy[topElement.getMessageSize() + 1];
            std::strcpy(dataCopy, topElement.getMessage().c_str());

            if (d_s) {
                int status = Cross_platform::cress_send(d_s, dataCopy, sizeof(dataCopy));

                if (status == -1) {
                    std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
                    // throw...
                }
            }

            m_min_heap.pop();
        }

        lock.unlock();
        
        // Wait for 1 second before continuing
        std::this_thread::sleep_for(std::chrono::seconds(40));
    }
}