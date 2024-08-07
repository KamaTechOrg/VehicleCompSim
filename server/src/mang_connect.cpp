#include "mang_connect.h"
#include <iostream>
#include <string.h>
#include <unistd.h> // For close()

static void add_fd_to_vect(std::vector<int> &clientSocket, FD new_socket)
{
    clientSocket.push_back(new_socket);
}

void Mange_connect::add_socket(FD new_socket)
{
    auto pair = create(new_socket);
    std::cout << "the fd is : " << new_socket << std::endl;

    std::unique_lock<std::mutex> lock(m_vec_client_mutex);
    add_fd_to_vect(m_client_fd, new_socket);
    lock.unlock();
    add_to_map(pair);
}

std::pair<int, std::shared_ptr<Socket>> Mange_connect::create(FD fd)
{
    auto new_socket = std::make_shared<Socket>();
    new_socket->set_FD(fd);

    void *data[MAXRECVID];

    new_socket->recv(data, MAXRECVID);
    new_socket->send(data, MAXRECVID);

    return std::make_pair(0, new_socket);
}

void Mange_connect::add_to_map(std::pair<int, std::shared_ptr<Socket>> pair)
{
    m_connections[pair.first] = pair.second;
}

std::shared_ptr<Socket> Mange_connect::get_sock(int id)
{
    return m_connections[id];
}

void Mange_connect::select_menger()
{
    int newSocket, maxSd, activity, sd, valread;
    fd_set readfds;
    char buffer[MAXRECV];

    while (true)
    {
        std::cout << "i fi " << std::endl;
        sleep(1);
        if (!m_client_fd.empty())
        {
            memset(buffer, 0, sizeof(buffer));
            FD_ZERO(&readfds);

            maxSd = 0;

            std::unique_lock<std::mutex> lock(m_vec_client_mutex);
            for (int i = 0; i < m_client_fd.size(); ++i)
            {
                sd = m_client_fd[i];
                if (sd > 0)
                    FD_SET(sd, &readfds);
                if (sd > maxSd)
                    maxSd = sd;
            }
            lock.unlock();
            std::cout << "i cum " << std::endl;
            activity = select(maxSd + 1, &readfds, NULL, NULL, NULL);
              std::cout << "i whit " << std::endl;
            if ((activity < 0) && (errno != EINTR))
            {
                perror("Select error");
                break;
            }

            for (int i = 0; i < m_client_fd.size(); ++i)
            {
                sd = m_client_fd[i];
                if (FD_ISSET(sd, &readfds))
                {
                    valread = recv(sd, buffer, sizeof(buffer), 0);
                    if (valread == 0)
                    {
                        // Connection closed
                        // getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                        // std::cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) << ", port " << ntohs(address.sin_port) << std::endl;

                        close(sd);
                        lock.lock();
                        m_client_fd.erase(m_client_fd.begin() + i);
                        lock.unlock();
                    
                    }
                    else if (valread > 0)
                    {
                        // Received data
                        buffer[valread] = '\0';
                        std::cout << "Received: " << buffer << std::endl;
                        // send(sd, buffer, strlen(buffer), 0);
                    }
                    else
                    {
                        perror("recv error");
                    }
                }
            }
        }
       
    }
}
