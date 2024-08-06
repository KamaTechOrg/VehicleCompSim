#include "mang_connect.h"
#include <iostream>
#include <string.h>

static void add_fd__to_vect(std::vector<int> &clientSocket, FD new_socket)
{
    for (int i = 0; i < MAXCONNECTIONS; i++)
    {
        if (clientSocket[i] == 0)
        {
            clientSocket[i] = new_socket;
            break;
        }
    }
}

void Mange_connect::add_socket(FD new_socket)
{

    auto pair = create(new_socket);
    std::cout << "the fd is : " << new_socket << std::endl;

    std::unique_lock<std::mutex> lock(m_vec_client_mutex);
    add_fd__to_vect(m_client_fd, new_socket);
    lock.unlock();
    add_to_map(pair);
}

std::pair<int, std::shared_ptr<Socket>> Mange_connect::create(FD fd)
{
    auto new_socket = std::make_shared<Socket>();
    new_socket->set_FD(fd);

    // check if need array ???/?????

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
    char buffer[MAXCONNECTIONS];

    while (true)
    {
        FD_ZERO(&readfds);

        maxSd = 0;

            std::unique_lock<std::mutex> lock(m_vec_client_mutex);
        for (int i = 0; i < 10; i++)
        {
           
            sd = m_client_fd[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > maxSd)
                maxSd = sd;
        }
             lock.unlock();

        activity = select(maxSd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            // break;
            perror("Select error");
        }

        int fff = 0;
        for (int i = 0; i < 10; i++)
        {
            sd = m_client_fd[i];
            if (FD_ISSET(sd, &readfds))
            {
                // std::cout << "fffffffffffffffffffff" << std::endl;
                (fff = recv(sd, buffer, 1024, 0));

                // buffer[valread] = '\0';
                std::cout << "Received: " << fff << buffer << std::endl;
                // send(sd, buffer, strlen(buffer), 0);
            }
            // std::cout << "ggggggggggggg" << std::endl;
        }
    }
}
