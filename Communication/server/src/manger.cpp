#include <chrono>

#include "manger.h"
#ifdef _WIN32
#define close_socket closesocket
#else
#define close_socket ::close
#endif

MangServer::MangServer() : m_server_eccept_socket{PORTSERVER}, m_recv_manger{*this}, m_send_manager{}
{
}

void MangServer::init()
{

    std::thread t_server(&MangServer::run_server, this);
    std::thread t_selector(&MangServer::run_recver, this);
    std::thread t_sender(&MangServer::run_sender, this);
    initialize_control();

    t_server.join();
    t_selector.join();
    t_sender.join();
}

void MangServer::run_server()
{
    while (true)
    {
        int fd = m_server_eccept_socket.wait_next_client();

        if (fd > 0)
        {
            int id = add_socket(fd);
            if (id != IDINNER)
            {
                char buf[10] = "msg";
                m_controll_socket.send(buf, sizeof(buf));
                m_recv_manger.notify();
            }
        }
    }
}

void MangServer::run_recver()
{
    m_recv_manger.schedule();
}

void MangServer::initialize_control()
{
    m_controll_socket.create();
    std::string ip_server = Data_manipulator::get_ip_server(IPFILENAME);
    m_controll_socket.connect(ip_server, PORTSERVER);

    std::string idStr = Data_manipulator::int_to_str(IDINNER);

    m_controll_socket.send((char *)idStr.c_str(), idStr.size());
}

void MangServer::run_sender()
{
    std::vector<CanBus> vec_canbus;
    auto get_sock_func = [this](int id) -> FD {
        return get_sock(id);
    };

    while (true)
    {
        m_send_manager.extractFromHeap(m_prioritysed_masseges_queue, m_queue_mutex , vec_canbus);
        m_send_manager.sendCanBusMessages(m_map_mutex , get_sock_func , vec_canbus);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int MangServer::add_socket(int new_socket)
{
    auto pair = create_kay_value_id(new_socket);

    std::unique_lock<std::mutex> lock(m_map_mutex);
    auto it = m_socket_connections.find(pair.first);
    if (it == m_socket_connections.end())
    {
        m_socket_connections[pair.first] = pair.second;
        Cross_platform::cress_send(pair.second, "OK", 3);
        std::cout << "Adding new socket with FD: " << new_socket << std::endl;
    }
    else
    {
        Cross_platform::cress_send(pair.second, "id_in use", 10);
        close_socket(new_socket);
    }
    lock.unlock();

    return pair.first;
}

std::pair<int, FD> MangServer::create_kay_value_id(int fd)
{
    char data[MAXRECVID];
    int size_recved = Cross_platform::cress_read(fd, data, 0);
    data[size_recved] = '\0';
    int id = atoi(data);

    return std::make_pair(id, fd);
}

FD MangServer::get_sock(int id)
{
    auto it = m_socket_connections.find(id);
    if (it != m_socket_connections.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}