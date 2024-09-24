
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define close_socket closesocket
#else
#include <unistd.h>
#define close_socket close
#endif


#include "receive_manger.h"

void Receive_manger::print_arr(std::unordered_map<int, FD> &m_connections)
{
    for (const auto &fd : m_connections)
    {
        std::cout << fd.second << "," << std::flush;
    }
}

void Receive_manger::handleActivity(fd_set &readfds, std::unordered_map<int, FD> &m_connections, char *buffer, 
                                    std::vector<CanBus> &vec_canbus, std::mutex &map_mutex)
{
    std::unique_lock<std::mutex> unordered_map_lock(map_mutex);

    for (auto it = m_connections.begin(); it != m_connections.end();)
    {
        int sd = it->second;
        if (FD_ISSET(sd, &readfds) && it->first != IDINNER)
        {
            readFromSocket(sd, it, buffer, vec_canbus, m_connections);
        }
        else if (FD_ISSET(sd, &readfds) && it->first == IDINNER)
        {
            Cross_platform::cress_read(it->second, buffer, 0); // Read for internal purposes
        }
        ++it;
    }
    unordered_map_lock.unlock();
}

void Receive_manger::readFromSocket(int sd, std::unordered_map<int, FD>::iterator &it, char *buffer, 
                                    std::vector<CanBus> &vec_canbus, std::unordered_map<int, FD> &m_connections)
{
    int valread = Cross_platform::cress_read(sd, buffer, 0);
    
    if (valread == 0)
    {
        close_socket(sd);
        it = m_connections.erase(it); // Remove connection if socket is closed
    }
    else if (valread > 0)
    {
        auto result = Data_manipulator::extract_id_and_data(buffer, valread);
        CanBus cb = result.value();
        writeCanMessageToLog(cb, LOGFILE);
        vec_canbus.push_back(cb);
    }
    else
    {
        perror("recv error");
    }
}

int Receive_manger::performSelect(fd_set &readfds, int max_sd)
{
    int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR))
    {
        perror("Select error");
    }
    return activity;
}

void Receive_manger::insert_fd(fd_set &set, int &max_fd, const std::unordered_map<int, FD> &unordered_map_fd)
{
    for (const auto &pair : unordered_map_fd)
    {
        int fd = pair.second;
        if (fd > 0)
        {
            FD_SET(fd, &set);
        }
        if (fd > max_fd)
        {
            max_fd = fd;
        }
    }
}

void Receive_manger::reset_in_loop(fd_set &set, int &fd, char *buf, int size)
{
    memset(buf, 0, size);
    FD_ZERO(&set);
    fd = 0;
}

void Receive_manger::wait_connect(std::mutex &map_mutex,std::unordered_map<int, FD> &m_connections){
    std::unique_lock<std::mutex> unordered_map_lock(map_mutex);

    // Wait until there's a connection to handle
    while (m_connections.empty())
    {
        m_condition.wait(unordered_map_lock);
    }
    

}

void Receive_manger::select_menger(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap, 
                                      std::mutex &heap_mutex, 
                                      std::mutex &map_mutex, 
                                      std::unordered_map<int, FD> &m_connections)
{
    int max_sd;
    fd_set readfds;
    char buffer[MAXRECV];
    std::vector<CanBus> vec_canbus;
    wait_connect(map_mutex , m_connections);

    // Main loop
    while (true)
    {
        reset_in_loop(readfds, max_sd, buffer, sizeof(buffer));
        print_arr(m_connections);

        // Insert file descriptors to be monitored
        std::unique_lock<std::mutex> unordered_map_lock(map_mutex);
        insert_fd(readfds, max_sd, m_connections);
        unordered_map_lock.unlock();

        // Perform select and handle activity
        int activity = performSelect(readfds, max_sd);
        if (activity <= 0) continue;

        handleActivity(readfds, m_connections, buffer, vec_canbus, map_mutex);

        // Insert CanBus data into the priority queue
        std::unique_lock<std::mutex> lock(heap_mutex);
        for (const auto &canbus : vec_canbus)
        {
            min_heap.push(canbus);
        }
        vec_canbus.clear();
        lock.unlock();
    }
}
