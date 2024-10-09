
#include "client.h"

// Function to send the client's ID to the server and listen for a confirmation response.
void ClientSocket::send_client_id()
{
    std::string idStr = Data_manipulator::int_to_str(my_id); // Convert ID to string.

    send((char *)idStr.c_str(), idStr.size(), my_id, my_id); // Send ID to the server.
    char buffer[20];
    listen(buffer, sizeof(buffer)); // Listen for confirmation.
    if (strcmp(buffer, "OK") != 0) // Check for expected response.
    {
        throw std::runtime_error(buffer); // Throw error if response is not "OK".
    }
}

// Constructor: Initializes a ClientSocket with a unique ID and connects to the server.
ClientSocket::ClientSocket(int id)
    : my_id{id}, m_clientSocket{std::make_shared<Socket>()}
{
    if (id <= 0)
    {
        throw std::invalid_argument("Invalid ID: must be a positive integer."); // Validate ID.
    }

    m_clientSocket->create(); // Create a socket.
    std::string ip_server = Data_manipulator::get_ip_server(IPFILENAME); // Get server IP.
    m_clientSocket->connect(ip_server, PORTSERVER); // Connect to the server.

    send_client_id(); // Send client ID to the server.
}

// Destructor: Closes the client socket if it exists.
ClientSocket::~ClientSocket()
{
    if (m_clientSocket)
        m_clientSocket->close(); // Close the socket.
}

// Send data to a specified destination client.
sendErrorCode ClientSocket::send(void *data, size_t size, int source_id, int dest_id)
{
    // Validate input parameters.
    if (!is_valid_ptr(data) || !is_valid_size(size) || !is_valid_d_id(dest_id))
    {   
        LOG_ERROR("invalid argument");
        throw std::runtime_error("Invalid input");
    }

    sendErrorCode code;
    // Prepare data for sending.
    auto pair_data = Data_manipulator::data_and_id_to_str(data, size, source_id, dest_id);
    auto clientSocket = m_clientSocket;

    if (clientSocket)
    {
        return code = clientSocket->send((void *)pair_data.first.c_str(), pair_data.second); // Send data.
    }
    code = sendErrorCode::SENDFAILED; // Set failure code if send fails.
    return code;
}

// Listen for incoming data and fill the provided buffer.
std::pair<ListenErrorCode, int> ClientSocket::listen(void *data, size_t size)
{
    auto clientSocket = m_clientSocket;

    // Validate input parameters.
    if (!is_valid_ptr(data) || !is_valid_size(size))
    {   
        LOG_ERROR("invalid argument");
        throw std::runtime_error("Invalid to receive");
    }

    std::pair<ListenErrorCode, int> pair_recv;
    if (clientSocket)
    {
        pair_recv = clientSocket->recv(data, size); // Receive data.
    }

    return pair_recv;
}

// Asynchronously listen for incoming data and invoke a callback with the result.
void ClientSocket::listenAsync(void *data, size_t size, std::function<void(ListenErrorCode)> callback)
{
    // Validate input parameters.
    if (!is_valid_ptr(data) || !is_valid_size(size))
    {   
        LOG_ERROR("invalid argument");
        throw std::runtime_error("Invalid to receive");
    }

    auto clientSocket = m_clientSocket;
    if (clientSocket)
    {
        // Create a new thread for asynchronous listening.
        std::thread t1([&clientSocket, this, data, size, callback]()
                       {
            auto pair_recv = clientSocket->recv(data, size); // Receive data.
            if (callback)
            {
                callback(pair_recv.first); // Call the callback with the result.
            } });

        t1.detach(); // Detach the thread to allow it to run independently.
    }
}

// Check if the pointer is valid.
bool ClientSocket::is_valid_ptr(void *ptr)
{
    return ptr; // Return true if pointer is not null.
}

// Check if the size is valid.
bool ClientSocket::is_valid_size(size_t size)
{
    return size <= MAXRECV; // Return true if size is within limits.
}

// Shutdown the client socket by nullifying the socket pointer.
void ClientSocket::shut_down()
{
    m_clientSocket = nullptr; // Set socket to null.
}

// Check if the destination ID is valid.
bool ClientSocket::is_valid_d_id(int d_id)
{
    return d_id > 0; // Return true if ID is positive.
}
