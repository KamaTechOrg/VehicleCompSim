#include "receive_manger.h"
#include "constants.h"
#include <iostream>
#include <string.h>
#include <unistd.h> // For close()
#include <mutex>
#include <cstring>

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

static void add_fd_to_vect(std::vector<int> &clientSocket, int new_socket)
{
    clientSocket.push_back(new_socket);
}

Receive_manger::Receive_manger()
{
    // m_my_socket.create();
    // m_my_socket.connect(IPSERVER, PORTSERVER);
    // add_fd_to_vect(m_client_fd, m_my_socket.get_FD());
}

void Receive_manger::add_socket(int new_socket)
{
    auto pair = create(new_socket);

    std::unique_lock<std::mutex> lock(m_vec_client_mutex);
    add_fd_to_vect(m_client_fd, new_socket);
    add_to_map(pair);
    lock.unlock();
    std::cout << "Adding new socket with FD: " << new_socket << std::endl;
}

std::pair<int, std::shared_ptr<Socket>> Receive_manger::create(int fd)
{
    auto new_socket = std::make_shared<Socket>();
    new_socket->set_FD(fd);

    char data[MAXRECVID];
    int a = new_socket->recv(data, MAXRECVID);
    data[a] = '\0';
    new_socket->send(data, MAXRECVID);
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
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    while (true)
    {
        std::cout << "Inside select loop" << std::endl;
        sleep(1);

        if (!m_client_fd.empty())
        {
            memset(buffer, 0, sizeof(buffer));
            FD_ZERO(&readfds);

            max_sd = 0;

            std::unique_lock<std::mutex> lock(m_vec_client_mutex);
            for (const auto &fd : m_client_fd)
            {
                if (fd > 0)
                {
                    FD_SET(fd, &readfds);
                }
                if (fd > max_sd)
                {
                    max_sd = fd;
                }
            }
            lock.unlock();
            for (auto &fd : m_client_fd)
            {
                std::cout << fd << "," << std::flush;
            }
            activity = select(max_sd + 1, &readfds, NULL, NULL, &tv);
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
                    valread = ::recv(sd, buffer, sizeof(buffer), 0);

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

                            char dataCopy[pair.second.size() + 1];
                            std::strcpy(dataCopy, pair.second.c_str());
                            int localdestid = pair.first;
                            std::cout << " " << pair.second << std::endl;
                            auto d_s = get_sock(localdestid);
                            int status = ::send(d_s->get_FD(), dataCopy, pair.second.size() + 1, MSG_NOSIGNAL);
                            if (status == -1)
                            {
                                std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
                                // throw...
                            }
                        }

                        //   buffer[valread] = '\0';
                        // std::cout << "Received: " << buffer << std::endl;
                        // send(sd, buffer, valread, MSG_NOSIGNAL);
                        // int a =send(sd, buffer, valread, MSG_NOSIGNAL);
                        // if (a == -1){
                        //     std::cout << "send error: " << errno<< std::endl;
                        // }
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
