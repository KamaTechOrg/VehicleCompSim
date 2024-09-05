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

class Send_manager{
    public:
    void operator ()(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap, 
        std::mutex &heap_mutex,std::mutex & map_mutex, 
        std::function<FD(int)> get_sock
    );
    
};