#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <mutex>

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

static std::vector<std::pair<int, std::string>> extractid_and_data(char *data, int len , std::priority_queue<CanBus, std::vector<CanBus> , CompareCanBus>& min_heap)
{
    std::vector<std::pair<int, std::string>> result;
    std::string datatosend;
    int sourceid = 0;
    int destid = 0;
    int identify = 0;

    for (int i = 0; i < len; ++i)
    {
        if (data[i] != '!')
        {
            datatosend += data[i];
        }
        else
        {
            // Process the current datatosend based on identify
            if (identify == 0)
            {
                
                
                    std::cout << "Source ID1111111: " << datatosend << std::endl;
                    sourceid = std::stoi(datatosend);
                
            }
            else 
            {
              
                
                    std::cout << "Destination ID222222222: " << datatosend << std::endl;
                    destid = std::stoi(datatosend);
                
            }
          

            datatosend.clear();
            identify += 1; // Reset after processing 3 segments
        }
    }

    // Handle the case where the last data segment isn't followed by '!'
    if (identify == 2 && !datatosend.empty())
    {
        result.emplace_back(destid, datatosend);
        CanBus cb (sourceid , destid , datatosend , datatosend.size());
        min_heap.push(cb);
        CanBus ff = min_heap.top();
        std::cout << "Top element in min heap: " << ff.getSourceId() << std::endl;

    }

    

    return result;
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
    std::cout << "received " << id << std::endl;

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

void Receive_manger::select_menger(std::priority_queue<CanBus, std::vector<CanBus> , CompareCanBus>& min_heap)
{
    int max_sd, activity, sd, valread;
    fd_set readfds;
    char buffer[MAXRECV];

    while (true)
    {
        if (!m_connections.empty())
        {
            reset_in_loop(readfds, max_sd, buffer, sizeof(buffer));

            std::unique_lock<std::mutex> lock(m_map_mutex);
            insert_fd(readfds, max_sd, m_connections);
            lock.unlock();

            print_arr();

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
                if (FD_ISSET(sd, &readfds))
                {
                    valread = cress_read(sd, buffer, 0);

                    std::vector<std::pair<int, std::string>> result = extractid_and_data(buffer, valread , min_heap);
                    

                    if (valread == 0)
                    {
                        close(sd);
                        std::cout << "Client disconnected, FD: " << sd << std::endl;

                       
                        it = m_connections.erase(it);
                    }
                    else if (valread > 0)
                    {
                        for (auto& pair : result)
                        {
                            std::cout << pair.second << std::endl;

                            char dataCopy[pair.second.size() + 1];
                            std::strcpy(dataCopy, pair.second.c_str());

                            if (dataCopy != "msg")
                            {
                                int localdestid = pair.first;
                                std::cout << pair.second << std::endl;
                                auto d_s = get_sock(localdestid);
                                if (d_s)
                                {
                                    int status = cress_send(d_s, dataCopy, sizeof(dataCopy));

                                    if (status == -1)
                                    {
                                        std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
                                        // throw...
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        perror("recv error");
                    }
                }
                
                ++it;
                
            }
            lock.unlock();
        }
    }
}
