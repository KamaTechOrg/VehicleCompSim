#pragma once

#include <future>
#include <thread>
#include <algorithm>


//#include "Logger.h"
#include "log_API/include/Logger.h"
#include "socket.h"
#include "constants.h"
#include "data_manipulator.h"

class ClientSocket
{
public:
    // Constructor with client ID.
    ClientSocket(int id);
    
    // Destructor.
    ~ClientSocket();

    // Send data to a specified client.
    sendErrorCode send(void *data, size_t size, int source_id, int dest_id);

    // Listen for incoming data.
    std::pair<ListenErrorCode, int> listen(void *data, size_t size);

    // Asynchronous listening with a callback.
    void listenAsync(void *data, size_t size, std::function<void(ListenErrorCode)> callback);

    // Shutdown the client socket.
    void shut_down();

private:

    // Function to send the client's ID to the server and listen for a confirmation response.
    void send_client_id();
    // Check if the pointer is valid.
    bool is_valid_ptr(void *ptr);

    // Check if the size is valid.
    bool is_valid_size(size_t size);

    // Check if the destination ID is valid.
    bool is_valid_d_id(int id);

    // Socket for client connection.
    std::shared_ptr<Socket> m_clientSocket;

    // Client ID.
    int my_id;
};
