#include "receive_manger.h"
#include <iostream>
#include <string.h>
#include <unistd.h> // For close()
#include <mutex>

static void add_fd_to_vect(std::vector<int>& clientSocket, int new_socket) {
    clientSocket.push_back(new_socket);
}

void Receive_manger::add_socket(int new_socket) {
    auto pair = create(new_socket);
    std::cout << "Adding new socket with FD: " << new_socket << std::endl;

    std::unique_lock<std::mutex> lock(m_vec_client_mutex);
    add_fd_to_vect(m_client_fd, new_socket);
    lock.unlock();
    add_to_map(pair);
}



std::pair<int, std::shared_ptr<Socket>> Receive_manger::create(int fd)
{
    auto new_socket = std::make_shared<Socket>();
    new_socket->set_FD(fd);

    char data[MAXRECVID];
    new_socket->recv(data, MAXRECVID);
    new_socket->send(data, MAXRECVID);

    return std::make_pair(fd, new_socket);
}

void Receive_manger::add_to_map(std::pair<int, std::shared_ptr<Socket>> pair) {
    std::unique_lock<std::mutex> lock(m_map_mutex);
    m_connections[pair.first] = pair.second;
}

std::shared_ptr<Socket> Receive_manger::get_sock(int id) {
    std::unique_lock<std::mutex> lock(m_map_mutex);
    return m_connections[id];
}

void Receive_manger::select_menger() {
    int max_sd, activity, sd, valread;
    fd_set readfds;
    char buffer[MAXRECV];

    while (true) {
        std::cout << "Inside select loop" << std::endl;
        sleep(1);

        if (!m_client_fd.empty()) {
            memset(buffer, 0, sizeof(buffer));
            FD_ZERO(&readfds);

            max_sd = 0;

            std::unique_lock<std::mutex> lock(m_vec_client_mutex);
            for (const auto& fd : m_client_fd) {
                if (fd > 0) {
                    FD_SET(fd, &readfds);
                }
                if (fd > max_sd) {
                    max_sd = fd;
                }
            }
            lock.unlock();

            activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
            if ((activity < 0) && (errno != EINTR)) {
                perror("Select error");
                break;
            }

            lock.lock();
            for (size_t i = 0; i < m_client_fd.size(); ++i) {
                sd = m_client_fd[i];
                if (FD_ISSET(sd, &readfds)) {
                    valread = recv(sd, buffer, sizeof(buffer), 0);
                    if (valread == 0) {
                        // Connection closed
                        close(sd);
                        m_client_fd.erase(m_client_fd.begin() + i);
                        std::cout << "Client disconnected, FD: " << sd << std::endl;
                    } else if (valread > 0) {
                        buffer[valread] = '\0';
                        std::cout << "Received: " << buffer << std::endl;
                        send(sd, buffer, valread, 0);
                    } else {
                        perror("recv error");
                    }
                }
            }
            lock.unlock();
        }
    }
}
