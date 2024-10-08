
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
<<<<<<< HEAD
        if(isCrcValid(topElement)){
            vec_can.push_back(topElement);
=======

        if(check_crc(topElement)){
            for(int i = 0; i < 100; i++){
                vec_can.push_back(topElement);
            }
            
>>>>>>> 229870f (fix recv mesege)
        }
        else{
            std::cout << "CRC check failed for canbus" << std::endl;
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
        // std::string crcstr = "%";
        // crcstr += Data_manipulator::int_to_str(canbus.crc);

        // size_t crc_len = crcstr.size();
        char data[MAXRECV];


        memcpy(data, canbus.getMessage().c_str(), message_len);


        // memcpy(data + message_len, crcstr.c_str(), crc_len);

        if (d_s)
        {   

            int status = Cross_platform::cress_send(d_s, data, message_len );

            if (status == -1)
            {
                std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
            }
        }

        std::string mesegeForGui = CanMessageToLog(canbus);
        FD d_s_gui = get_sock(100);

        if (d_s_gui)
        {   
            std::cout << d_s_gui << "gguuii" << std::endl;
            int status = Cross_platform::cress_send(d_s_gui, mesegeForGui.c_str(), mesegeForGui.size());

            if (status == -1)
            {
                std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
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
    return (crc2 == can.crc);
}
