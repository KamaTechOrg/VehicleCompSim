
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
        char * data =(char*)canbus.getMessage().c_str();

        if (d_s)
        {
            int status = Cross_platform::cress_send(d_s, data, canbus.getMessageSize());

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
