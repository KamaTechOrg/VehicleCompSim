#pragma once

#include <condition_variable>
#include <unordered_map>
#include <iostream>
#include <iostream>
#include <cstring>
#include <cstring>
#include <mutex>
#include <vector>
#include <memory>
#include <queue>



#include "socket.h"
#include "canbus.h"
#include "constants.h"
#include "data_manipulator.h"
#include "socket_cross_platform.h"

class Receive_manger
{
public:
    void select_menger(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap, std::mutex &heap_mutex, std::mutex &map_mutex, std::unordered_map<int, FD> &m_connections);
    std::condition_variable m_condition;

private:
    void wait_connect(std::mutex &map_mutex,std::unordered_map<int, FD> &m_connections);
    void print_arr(std::unordered_map<int, FD> & m_connections);
    void handleActivity(fd_set &readfds, std::unordered_map<int, FD> &m_connections, char *buffer,
                        std::vector<CanBus> &vec_canbus, std::mutex &map_mutex);
    
    void readFromSocket(int sd, std::unordered_map<int, FD>::iterator &it, char *buffer,
                        std::vector<CanBus> &vec_canbus, std::unordered_map<int, FD> &m_connections);
    
    int performSelect(fd_set &readfds, int max_sd);
    
    void insert_fd(fd_set &set, int &max_fd, const std::unordered_map<int, FD> &unordered_map_fd);
    
    void reset_in_loop(fd_set &set, int &fd, char *buf, int size);
};
