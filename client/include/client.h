#pragma once

#include <future>

#include "socket.h"
#include "constants.h"
#include "data_manipulator.h"


class ClientSocket
{

public:
    ClientSocket( int id);

    void send(void *data, size_t size, int source_id, int dest_id);
    void listen(void *data, size_t size);
    std::future<void> listenAsync(void *data, size_t size, std::function<void()> callback);

private:
    bool is_valid_ptr(void *ptr);
    bool is_valid_size(size_t size);
    void shut_down();

    Socket m_clientSocket;
    int my_id;
    
};