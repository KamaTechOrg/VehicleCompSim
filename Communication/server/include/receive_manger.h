#pragma once

#include <condition_variable>
#include <unordered_map>
#include <iostream>
#include <iostream>
#include <cstring>
#include <cstring>
#include <mutex>
#include <vector>
#include <memory>
#include <queue>

#include "socket.h"
#include "canbus.h"
#include "Logger.h"
#include "constants.h"
#include "data_manipulator.h"
#include "socket_cross_platform.h"

typedef std::unordered_map<int, FD>::iterator ItMap;


class MangServer ;

// Manager class for handling receiving operations using select()
class ReceiveManager
{
public:
    //ctor
    ReceiveManager(MangServer & server):m_mangServer{server}{};
    // Function to manage socket activity using the select() mechanism
    void schedule();

     // Function to notify threads waiting on the condition variable
    void notify();
private:
    // reference to db contaner
    MangServer & m_mangServer;
    // Condition variable to notify waiting threads of changes
    std::condition_variable m_connection_cv;

    // Function to wait until there's a connection to handle
    void waitForConnection();

    // Function to print details of the connection map
    void printConnectionMap();

    // Function to handle activity on the sockets in the read file descriptor set
    void handleActivity(fd_set &readfds,std::vector<CanBus> &vec_canbus);
    
    // Function to read data from a socket and process it
    void readFromSocket(int sd, ItMap& it_map, char *buffer,
                        std::vector<CanBus> &vec_canbus);
    
    // Function to perform the select operation and return the number of active file descriptors
    int performSelect(fd_set &readfds, int max_sd);
    
    // Function to insert file descriptors into the set for the select call and update the max file descriptor value
    void insert_fd(fd_set &set, int &max_fd);
    
    // Function to reset the file descriptor set, file descriptor, and buffer during each loop iteration
    void reset_in_loop(fd_set &set, int &fd);

    // Insert CanBus data into the priority queue
    void insertCanBusFromvectorToQueue(std::vector<CanBus>& vec_canbus);

};
