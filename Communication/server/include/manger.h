#pragma once

#include <thread>

#include "server.h"
#include "receive_manger.h"
#include "send_menger.h"

// Manager class for handling server operations, managing connections, and sending/receiving data
class MangServer 
{
public:
    // Constructor to initialize the MangServer class
    MangServer();

    // Public function to initialize the server
    void init();
    
private:
    // Private function to run the server and accept incoming connections
    void run_server();

    // Function to manage the receiving process
    void run_recver();

    // Function to manage sending data to clients
    void run_sender();

    // Helper function to perform additional initialization
    void initialize_control();

    // Add a new socket to the list of connections and return its ID
    int add_socket(int new_socket);

    // Create a key-value pair using a file descriptor (fd)
    std::pair<int, FD> create_kay_value_id(int fd);

    // Retrieve a socket/file descriptor using a given connection ID
    FD get_sock(int id);

    // Server object to accept incoming socket connections
    Server m_server_eccept_socket;

    // Socket used for controlling operations (e.g., administrative commands)
    Socket m_controll_socket;

    // Manager for receiving data from clients
    ReceiveManager m_recv_manger;

    // Manager for sending data to clients
    SendManager m_send_manager;

    // Priority queue for managing messages to be sent, with CanBus objects
    std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> m_prioritysed_masseges_queue;

    // Mapping of connections where the key is the connection ID and the value is a file descriptor
    std::unordered_map<int, FD> m_socket_connections;

    // Mutex to protect access to the priority queue
    std::mutex m_queue_mutex;

    // Mutex to protect access to the connections map
    std::mutex m_map_mutex;

    friend class ReceiveManager;
};
