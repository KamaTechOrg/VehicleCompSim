#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <mutex>

#include "data_manipulator.h"
#include "receive_manger.h"
#include "constants.h"
#include "canbus.h"

static void insert_fd(fd_set &set, int &max_fd, std::map<int, FD> map_fd)
{
    for (const auto &pair : map_fd)
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

static int cress_read(int sd, char *buf, int flags)
{
#ifdef _WIN32
    return ::recv(sd, buf, MAXRECV, 0);
#else
    return ::recv(sd, buf, MAXRECV, 0);
#endif
}

static int cress_send(FD d_s, char *buf, size_t size)
{
#ifdef _WIN32
    return ::send(d_s, static_cast<const char *>(buf), static_cast<int>(size), 0);
#else
    return ::send(d_s, buf, size + 1, MSG_NOSIGNAL);
#endif
}

int Receive_manger::add_socket(int new_socket)
{
    auto pair = create_kay_value_id(new_socket);

    std::unique_lock<std::mutex> lock(m_map_mutex);
    auto it = m_connections.find(pair.first);
    if (it == m_connections.end())
    {
        m_connections[pair.first] = pair.second;
        cress_send(pair.second, "OK", 3);
        std::cout << "Adding new socket with FD: " << new_socket << std::endl;
    }
    else
    {
        cress_send(pair.second, "id_in use", 10);
        ::close(new_socket);
    }
    lock.unlock();

    return pair.first;
}

void Receive_manger::print_arr()
{
    for (auto &fd : m_connections)
    {
        std::cout << fd.second << "," << std::flush;
    }
}

std::pair<int, FD> Receive_manger::create_kay_value_id(int fd)
{
    char data[MAXRECVID];
    int size_recved = cress_read(fd, data, 0);
    data[size_recved] = '\0';
    int id = atoi(data);

    return std::make_pair(id, fd);
}

FD Receive_manger::get_sock(int id)
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

void Receive_manger::select_menger(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap , std::mutex &heap_mutex)
{
    int max_sd, activity, sd, valread;
    fd_set readfds;
    char buffer[MAXRECV];

    std::unique_lock<std::mutex> lock(m_map_mutex);

    while (m_connections.empty())
    {
        m_condition.wait(lock);
    }
    lock.unlock();

    while (true)
    {
        reset_in_loop(readfds, max_sd, buffer, sizeof(buffer));
        print_arr();

        lock.lock();
        insert_fd(readfds, max_sd, m_connections);
        lock.unlock();

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("Select error");
            break;
        }

        lock.lock();
        for (auto it = m_connections.begin(); it != m_connections.end();)
        {
            sd = it->second;
            if (FD_ISSET(sd, &readfds) && it->first != IDINNER)
            {
                valread = cress_read(sd, buffer, 0);

                if (valread == 0)
                {
                    close(sd);
                    it = m_connections.erase(it);
                }
                else if (valread > 0)
                {
                    auto result = Data_manipulator::extract_id_and_data(buffer, valread);

                    CanBus cb = result.value();
                    std::unique_lock<std::mutex> lock(heap_mutex);
                    min_heap.push(cb);
                    CanBus cc = min_heap.top();
                    std::cout << "top = " << cc.getSourceId()  << std::endl;
                    std::cout << " sizeheap = " << min_heap.size() << std::endl;
                    lock.unlock();
                    std::cout << min_heap.size() << std::endl;

                  
                }
                else
                {
                    perror("recv error");
                }
            }
            else if (FD_ISSET(sd, &readfds) && it->first == IDINNER)
            {
                cress_read(it->second, buffer, 0);
            }
            ++it;
        }
        lock.unlock();
    }
}
