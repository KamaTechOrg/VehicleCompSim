#pragma once

#include "socket.h"
#include "constants.h"
#include "data_manipulator.h"


class ClientSocket
{

public:
    ClientSocket( int id);

    void send(void *data, size_t size, int source_id, int dest_id);
    void listen(void *data, size_t size);
    void listen_async(void *data, size_t size) const;

   

private:
    bool is_valid_ptr(void *ptr);
    bool is_valid_size(size_t size);
    void shut_down();

    Socket m_clientSocket;
    int my_id;
    
};