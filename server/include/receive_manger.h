#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>
#include <memory>
#include <map>
#include <queue>

#include "socket.h"
#include "canbus.h"

class Receive_manger {
public:
    
    int add_socket(int new_socket);
    void select_menger(std::priority_queue<CanBus, std::vector<CanBus> , std::greater<CanBus>>& min_heap , std::mutex &heap_mutex );
    std::condition_variable m_condition;
    FD get_sock(int id);
    std::mutex m_map_mutex;   

private:
    
    std::pair<int, FD> create_kay_value_id(int fd);
    void print_arr();
   
    std::map<int, FD> m_connections; 
     
};
