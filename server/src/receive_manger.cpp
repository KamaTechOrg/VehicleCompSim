#include <iostream>
#include <string.h>
#include <cstring>
#ifdef _WIN32
// Windows-specific includes
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define close_socket closesocket

#else
// Unix-like specific includes
#include <unistd.h>
#define close_socket close

#endif

#include <mutex>

#include "receive_manger.h"


static void insert_fd(fd_set &set, int &max_fd, std::unordered_map<int, FD> unordered_map_fd)
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

static void reset_in_loop(fd_set &set, int &fd, char *buf, int size)
{
    memset(buf, 0, size);
    FD_ZERO(&set);
    fd = 0;
}


void Receive_manger::print_arr(std::unordered_map<int, FD> m_connections)
{
    for (auto &fd : m_connections)
    {
        std::cout << fd.second << "," << std::flush;
    }
}

void Receive_manger::select_menger(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap , std::mutex &heap_mutex , std::mutex & map_mutex, std::unordered_map<int, FD> & m_connections  )
{
    int max_sd, activity, sd, valread;
    fd_set readfds;
    char buffer[MAXRECV];

    std::unique_lock<std::mutex> unordered_map_lock(map_mutex);
   
    while (m_connections.empty())
    {
        m_condition.wait(unordered_map_lock);
    }
    unordered_map_lock.unlock();

    while (true)
    {
        reset_in_loop(readfds, max_sd, buffer, sizeof(buffer));
        print_arr(m_connections);

        unordered_map_lock.lock();
        insert_fd(readfds, max_sd, m_connections);
        unordered_map_lock.unlock();

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("Select error");
            break;
        }

        unordered_map_lock.lock();
        for (auto it = m_connections.begin(); it != m_connections.end();)
        {
            sd = it->second;
            if (FD_ISSET(sd, &readfds) && it->first != IDINNER)
            {
                valread = Cross_platform::cress_read(sd, buffer, 0);

                if (valread == 0)
                {
                    close_socket(sd);
                    it = m_connections.erase(it);
                }
                else if (valread > 0)
                {
                    auto result = Data_manipulator::extract_id_and_data(buffer, valread);

                    CanBus cb = result.value();
                    writeCanMessageToLog(cb , LOGFILE);
                    std::unique_lock<std::mutex> lock(heap_mutex);
                    min_heap.push(cb);
                    CanBus cc = min_heap.top();
                    std::cout << "top = " << cc.getSourceId()  << std::endl;
                    std::cout << " sizeheap = " << min_heap.size() << std::endl;
                    std::cout << min_heap.size() << std::endl;
                    lock.unlock();

                  
                }
                else
                {
                    perror("recv error");
                }
            }
            else if (FD_ISSET(sd, &readfds) && it->first == IDINNER)
            {
                Cross_platform::cress_read(it->second, buffer, 0);
            }
            ++it;
        }
        unordered_map_lock.unlock();
    }
}
