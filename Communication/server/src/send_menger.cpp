
#include "send_menger.h"
#include <Logger.h>

void SendManager::extractFromHeap(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap,
            std::mutex &heap_mutex,
            std::vector<CanBus> &vec_can
            )
{
    Locker lock(heap_mutex);

    while (!min_heap.empty())
    {
        CanBus topElement = min_heap.top();
        if(isCrcValid(topElement)){
            vec_can.push_back(topElement);
        }
        else{
            LOG_WARN("CRC check failed for canbus");
        }
        min_heap.pop();
    }
}

void SendManager::sendCanBusMessages(std::mutex &map_mutex, std::function<FD(int)> get_sock, std::vector<CanBus> &vec_can)
{
    Locker lock(map_mutex);

    for (auto canbus : vec_can)
    {
        FD d_s = get_sock(canbus.getDestinationId());
        size_t message_len = canbus.getMessageSize();
        
        char data[MAXRECV];

        memcpy(data, canbus.getMessage().c_str(), message_len);

        if (d_s)
        {   

            int status = Cross_platform::cress_send(d_s, data, message_len);

            if (status == -1)
            {
                std::stringstream ss;
                ss << "status == -1" << errno;
                LOG_INFO(ss.str());
            }
        }

        std::string mesegeForGui = CanMessageToLog(canbus);
        FD d_s_gui = get_sock(100);

        if (d_s_gui)
        {   
            int status = Cross_platform::cress_send(d_s_gui, mesegeForGui.c_str(), mesegeForGui.size());

            if (status == -1)
            {
                std::stringstream ss;
                ss << "status == -1" << errno;
                LOG_INFO(ss.str());
            }
        }
    }
    vec_can.clear();
}

bool SendManager::isCrcValid(CanBus can)
{
    
    std::string myString(can.getMessage());
    char *modifiableCharPtr = const_cast<char*>(myString.c_str());
    int crc2 = Data_manipulator::CRCalgo(modifiableCharPtr);
    return (crc2 == can.getCrc());
}
