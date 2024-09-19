#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>
#include <memory>
#include <unordered_map>
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
    void print_arr(std::unordered_map<int, FD> m_connections);
};
