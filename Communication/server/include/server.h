#pragma once

#include "socket.h"

// Class representing a server that listens for incoming client connections
class Server
{
public:
    // Constructor that initializes the server with a specific port
    Server(int port);
    
    // Function to wait for the next client connection and return the file descriptor for the connected socket
    FD wait_next_client();

private:
    int m_id;                     // Unique identifier for the server instance
    Socket m_server_socket;       // Socket object used for listening to client connections
};
