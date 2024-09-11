
#include "send_menger.h"

void Send_manager::operator()(std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap,
                              std::mutex &heap_mutex, std::mutex &map_mutex,
                              std::function<FD(int)> get_sock)
{
    std::unique_lock<std::mutex> heap_lock(heap_mutex);

    while (!min_heap.empty())
    {

        CanBus topElement = min_heap.top();

        // std::unique_lock<std::mutex> lock(map_mutex);
        auto d_s = get_sock(topElement.getDestinationId());
        // lock.unlock();
        int size = topElement.getMessageSize() + 1;
        std::vector<char> buffer(size);
        char* dataCopy = buffer.data();
        std::strcpy(dataCopy, topElement.getMessage().c_str());

        if (d_s)
        {
            int status = Cross_platform::cress_send(d_s, dataCopy, sizeof(dataCopy));

            if (status == -1)
            {
                std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
                // throw...
            }
        }
        min_heap.pop();
    }
    heap_lock.unlock();
}
