#include <chrono>

#include "manger.h"
#ifdef _WIN32
#define close_socket closesocket
#else
#define close_socket ::close
#endif

MangServer::MangServer() : m_server{PORTSERVER}, m_req{}, m_connect{}, m_send_manager{}
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

            int id = add_socket(fd);
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
    m_connect.select_menger(m_min_heap, m_heap_mutex, m_map_mutex, m_connections);
}

void MangServer::init_inner()
{
    m_server_socket.create();
    m_server_socket.connect(IPSERVER, PORTSERVER);

    std::string idStr = Data_manipulator::int_to_str(IDINNER);

    m_server_socket.send((char *)idStr.c_str(), idStr.size());
}

void MangServer::run_sender()
{
    auto get_sock_func = [this](int id) -> FD {
        return get_sock(id);
    };

    while (true)
    {
        m_send_manager(m_min_heap, m_heap_mutex, m_map_mutex, get_sock_func);
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

int MangServer::add_socket(int new_socket)
{
    auto pair = create_kay_value_id(new_socket);

    std::unique_lock<std::mutex> lock(m_map_mutex);
    auto it = m_connections.find(pair.first);
    if (it == m_connections.end())
    {
        m_connections[pair.first] = pair.second;
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
    auto it = m_connections.find(id);
    if (it != m_connections.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}