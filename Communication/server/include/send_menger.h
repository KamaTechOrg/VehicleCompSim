#pragma once

#include <mutex>
#include <queue>
#include <functional>

#include "canbus.h"
#include "socket.h"
#include "constants.h"
#include "socket_cross_platform.h"

// Manager class for handling sending operations of CanBus messages
class SendManager {
public:
    // Function to extract messages from the priority queue and store them in a vector
    void extractFromHeap(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap, 
                      std::mutex &heap_mutex, 
                      std::vector<CanBus>& vec_can);

    // Function to send messages from a vector to the appropriate socket
    void sendCanBusMessages(std::mutex &map_mutex, std::function<FD(int)> get_sock, std::vector<CanBus>& vec_can);

    // Function to check the CRC (Cyclic Redundancy Check) of a CanBus message
    bool isCrcValid(CanBus can);
};
