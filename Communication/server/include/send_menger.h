#pragma once

#include <mutex>
#include <queue>
#include <canbus.h>
#include <iostream>
#include <cstring> 
#include <functional>

#include "canbus.h"
#include "constants.h"
#include "socket.h"
#include "socket_cross_platform.h"
#include <map>

class Send_manager {
public:
    void extract_heap(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap, 
        std::mutex &heap_mutex, 
        std::vector<CanBus>& vec_can
    );

    void send_vector(std::mutex &map_mutex,  std::function<FD(int)> get_sock , std::vector<CanBus>& vec_can);

    bool check_crc(CanBus can);
};