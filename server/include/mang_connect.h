#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "mang_req.h"
#include "socket.h"
#include <mutex>

class Mange_connect
{
public:

    void add_socket(FD);
    std::pair<int,std::shared_ptr<Socket>> create(FD);
    void add_to_map(std::pair<int,std::shared_ptr<Socket>>);
    int get_sock(int);

    void select_menger();

   int extractid(char * );

   std::vector<std::pair<int, char*>> extractid_and_data(char* data , int len);




private:

    std::mutex m_vec_client_mutex;

    std::unordered_map<int, std::shared_ptr<Socket>> m_connections;
    std::vector<int> m_client_fd ;
    Mamge_req m_req;
};