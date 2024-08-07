#include "client.h"

#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
#include "constants.h"

ClientSocket::ClientSocket(int id)
  : my_id(id)
{
    m_clientSocket.create();
    m_clientSocket.connect(IPSERVER , PORTSERVER);
    std::string ss {"Hello"};
    m_clientSocket.send((char *)ss.c_str() ,ss.size());
    char buffer[20];
    m_clientSocket.recv(buffer, 20);
}

void ClientSocket::send(void *data, size_t size, int source_id, int dest_id)
{
    

    std::ostringstream oss;
    oss << source_id << '!' << dest_id << '!';
    std::string id_data = oss.str();

    
    id_data.append(static_cast<const char *>(data), size);

   
    void *AllData = (char *)(id_data.c_str());
    size_t total_size = id_data.size();

    
    m_clientSocket.send(AllData, total_size);
}

void ClientSocket::listen(void *data, size_t size)
{   
    // Receive the data from server
    void* buffer[MAXRECV];
    memset(buffer, 0, sizeof(buffer));
    // m_clientSocket.recv(buffer, sizeof(buffer));

    data = buffer;
 
}
