#pragma once
#include "../utilities/socket.h"


class ClientSocket
{

public:
    ClientSocket(const int id);

    void send(void *data, size_t size, int source_id, int dest_id);
    void send_async(void *data, size_t size, int dest_id, int source_id) const;

    void listen(void *data, size_t size);
    void listen_async(void *data, size_t size) const;

   

private:
    void shut_down();

    Socket m_clientSocket;
    
    int my_id;
    
};