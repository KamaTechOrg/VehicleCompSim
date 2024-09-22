#pragma once

#include <future>

#include "socket.h"
#include "constants.h"
#include "data_manipulator.h"


class ClientSocket
{

public:
    ClientSocket( int id);

    sendErrorCode send(void *data, size_t size, int source_id, int dest_id);
    std::pair<ListenErrorCode,int> listen(void *data, size_t size);
    void listenAsync(void *data, size_t size, std::function<void(ListenErrorCode)> callback);
    void shut_down();

private:
    bool is_valid_ptr(void *ptr);
    bool is_valid_size(size_t size);
    bool is_valid_d_id(int id);

    std::shared_ptr<Socket> m_clientSocket;
    int my_id;
    
};