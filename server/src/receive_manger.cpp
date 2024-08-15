#include "receive_manger.h"
#include "constants.h"

#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <mutex>

static void insert_fd(fd_set &set, int &max_fd, std::vector<int> &vec_fd)
{
    for (const auto &fd : vec_fd)
    {
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

static int cress_send(std::shared_ptr<Socket> d_s, char * buf, size_t size )
{
#ifdef _WIN32
   return ::send(d_s->get_FD(), static_cast<const char *>(buf), static_cast<int>(size), 0);
#else
    return ::send(d_s->get_FD(), buf, size + 1, MSG_NOSIGNAL);
#endif
}

static std::vector<std::pair<int, std::string>> extractid_and_data(char *data, int len)
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
                if (!datatosend.empty())
                {
                    std::cout << "Source ID: " << datatosend << std::endl;
                    sourceid = std::stoi(datatosend);
                }
            }
            else if (identify == 1)
            {
                if (!datatosend.empty())
                {
                    std::cout << "Destination ID: " << datatosend << std::endl;
                    destid = std::stoi(datatosend);
                }
            }
            else if (identify == 2)
            {
                if (!datatosend.empty())
                {
                    std::cout << "Data: " << datatosend << std::endl;
                    result.emplace_back(destid, datatosend);
                }
            }

            datatosend.clear();
            identify = (identify + 1) % 3; // Reset after processing 3 segments
        }
    }

    // Handle the case where the last data segment isn't followed by '!'
    if (identify == 2 && !datatosend.empty())
    {
        result.emplace_back(destid, datatosend);
    }

    return result;
}

void Receive_manger::add_socket(int new_socket)
{
    auto pair = create(new_socket);

    std::unique_lock<std::mutex> lock(m_vec_client_mutex);
    m_client_fd.push_back(new_socket);
    add_to_map(pair);
    lock.unlock();

    std::cout << "Adding new socket with FD: " << new_socket << std::endl;

    return pair.first;
}

void Receive_manger::print_arr()
{
    for (auto &fd : m_client_fd)
    {
        std::cout << fd << "," << std::flush;
    }
}

std::pair<int, std::shared_ptr<Socket>> Receive_manger::create(int fd)
{
    char data[MAXRECVID];

    auto new_socket = std::make_shared<Socket>();
    new_socket->set_FD(fd);
    int size_recved = new_socket->recv(data, MAXRECVID);
    data[size_recved] = '\0';
    int id = atoi(data);
    std::cout << "received " << id << std::endl;

    return std::make_pair(id, new_socket);
}

void Receive_manger::add_to_map(std::pair<int, std::shared_ptr<Socket>> pair)
{
    std::unique_lock<std::mutex> lock(m_map_mutex);
    m_connections[pair.first] = pair.second;
}

std::shared_ptr<Socket> Receive_manger::get_sock(int id)
{
    std::unique_lock<std::mutex> lock(m_map_mutex);
    return m_connections[id];
}

void Receive_manger::select_menger()
{
    int max_sd, activity, sd, valread;
    fd_set readfds;
    char buffer[MAXRECV];
    // struct timeval tv;
    // tv.tv_sec = 50;
    // tv.tv_usec = 0;
    while (true)
    {

        if (!m_client_fd.empty())
        {
            reset_in_loop(readfds, max_sd, buffer, sizeof(buffer));

            std::unique_lock<std::mutex> lock(m_vec_client_mutex);
            insert_fd(readfds, max_sd, m_client_fd);
            lock.unlock();

            print_arr();
            std::cout << " loop " << std::endl;

            activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
            if ((activity < 0) && (errno != EINTR))
            {
                perror("Select error");
                break;
            }

            lock.lock();
            for (size_t i = 0; i < m_client_fd.size(); ++i)
            {
                sd = m_client_fd[i];
                if (FD_ISSET(sd, &readfds))
                {
                    valread = cress_read(sd, buffer, 0);

                    std::vector<std::pair<int, std::string>> result = extractid_and_data(buffer, valread);

                    if (valread == 0)
                    {
                        // Connection closed
                        close(sd);
                        m_client_fd.erase(m_client_fd.begin() + i);
                        std::cout << "Client disconnected, FD: " << sd << std::endl;
                    }
                    else if (valread > 0)
                    {
                        
                        for (auto pair : result)
                        {
                            std::cout << pair.second << std::endl;
                            auto d_s = get_sock(pair.first);

                            char dataCopy[pair.second.size() + 1];
                            std::strcpy(dataCopy, pair.second.c_str());

                            if(dataCopy != "msg"){
                                  int localdestid = pair.first;
                            std::cout << " " << pair.second << std::endl;
                            auto d_s = get_sock(localdestid);
#ifdef _WIN32
                            int status = ::send(d_s->get_FD(), static_cast<const char *>(dataCopy), static_cast<int>(pair.second.size()) + 1, 0);
#else
                            int status = ::send(d_s->get_FD(), dataCopy, pair.second.size() + 1, MSG_NOSIGNAL);
#endif

                            if (status == -1)
                            {
                                std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
                                // throw...
                            }
                            }





                          
                        }
                    }
                    else
                    {
                        perror("recv error");
                    }
                }
            }
            lock.unlock();
        }
    }
}
