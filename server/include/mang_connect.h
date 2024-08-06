#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "socket.h"
#include <mutex>

class Mange_connect
{
public:

    void add_socket(FD);
    std::pair<int,std::shared_ptr<Socket>> create(FD);
    void add_to_map(std::pair<int,std::shared_ptr<Socket>>);
    std::shared_ptr<Socket> get_sock(int);

    void select_menger();


private:

    std::mutex m_vec_client_mutex;

    std::unordered_map<int, std::shared_ptr<Socket>> m_connections;
    std::vector<int> m_client_fd {MAXCONNECTIONS , 0};
};