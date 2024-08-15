#pragma once
#include "socket.h"


class ClientSocket
{

public:
    ClientSocket( int id);

    void send(void *data, size_t size, int source_id, int dest_id);

    void listen(void *data, size_t size);
    void listen_async(void *data, size_t size) const;

   

private:
    void shut_down();

    Socket m_clientSocket;
    
    int my_id;
    
};