#include "client.h"

#include <string>
#include <sstream>
#include <iostream>
#include <string.h>

ClientSocket::ClientSocket(const int id)
  : my_id(id)
{
    m_clientSocket.create();
    m_clientSocket.connect("localhost", 50000);
}

void ClientSocket::send(void *data, size_t size, int source_id, int dest_id)
{
     // check if destid is in database!!!

    std::ostringstream oss;
    oss << source_id << '!' << dest_id << '!';
    std::string id_data = oss.str();

    // Append the original data to the id_data
    id_data.append(static_cast<const char *>(data), size);

    // Convert the final string to void*
    void *AllData = static_cast<void *>(const_cast<char *>(id_data.c_str()));
    size_t total_size = id_data.size();

    
    m_clientSocket.send(AllData, total_size);
}

void ClientSocket::listen(void *data, size_t size)
{   
    // Receive the data from server
    void* buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    m_clientSocket.recv(buffer, sizeof(buffer));

    data = buffer;
 
}
