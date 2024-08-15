
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
#include <algorithm>

#include "client.h"
#include "constants.h"
#include "data_manipulator.h"


ClientSocket::ClientSocket(int id)
  : my_id(id)
{
    m_clientSocket.create();
    m_clientSocket.connect(IPSERVER , PORTSERVER);
    
    std::string idStr = Data_manipulator::int_to_str(id);

    m_clientSocket.send((char *)idStr.c_str() ,idStr.size());
   
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
   
    int len = m_clientSocket.recv(data, size);
}



