#pragma once

#include <unordered_map>
#include <memory>

#include "socket.h"

class Mange_connect
{
public:
    void add_socket(FD);
    std::pair<int,std::shared_ptr<Socket>> create(FD);
    void add_to_map(std::pair<int,std::shared_ptr<Socket>>);
    std::shared_ptr<Socket> get_sock(int);

private:
    std::unordered_map<int, std::shared_ptr<Socket>> m_connections;
};