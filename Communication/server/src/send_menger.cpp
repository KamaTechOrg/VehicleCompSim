
#include "send_menger.h"

void Send_manager::extract_heap(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap,
            std::mutex &heap_mutex,
            std::vector<CanBus> &vec_can
            )
{
    std::unique_lock<std::mutex> heap_lock(heap_mutex);

    while (!min_heap.empty())
    {
        CanBus topElement = min_heap.top();

        if(check_crc(topElement)){
            vec_can.push_back(topElement);
        }
        else{
            std::cout << "CRC check failed for canbus" << std::endl;
        }

        min_heap.pop();
    }
    heap_lock.unlock();
}

void Send_manager::send_vector(std::mutex &map_mutex, std::function<FD(int)> get_sock, std::vector<CanBus> &vec_can)
{
    std::unique_lock<std::mutex> lock(map_mutex);

    for (auto canbus : vec_can)
    {
        FD d_s = get_sock(canbus.getDestinationId());
        size_t message_len = canbus.getMessageSize();
        std::string crcstr = "%";
        crcstr += Data_manipulator::int_to_str(canbus.crc);

        size_t crc_len = crcstr.size();
        char data[message_len + crc_len];


        memcpy(data, canbus.getMessage().c_str(), message_len);


        memcpy(data + message_len, crcstr.c_str(), crc_len);
        if (d_s)
        {   

            int status = Cross_platform::cress_send(d_s, data, message_len + crc_len);

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
    lock.unlock();
}

bool Send_manager::check_crc(CanBus can)
{
    
    std::string myString(can.getMessage());
    char *modifiableCharPtr = const_cast<char*>(myString.c_str());
    int crc2 = Data_manipulator::CRCalgo(modifiableCharPtr);
    return (crc2 == can.crc);
}
