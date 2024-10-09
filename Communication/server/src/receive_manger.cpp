
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define close_socket closesocket
#else
#include <unistd.h>
#include <Logger.h>
#define close_socket close
#endif

#include "receive_manger.h"
#include "manger.h"
#include "helpers.h"

void ReceiveManager::printConnectionMap()
{
    for (const auto &fd : m_mangServer.m_socket_connections)
    {
        std::cout << fd.second << "," << std::flush;
    }
}

void ReceiveManager::handleActivity(fd_set &readfds, std::vector<CanBus> &vec_canbus)
{
    char buffer[MAXRECV];
    Locker lock(m_mangServer.m_map_mutex);

    for (auto it = m_mangServer.m_socket_connections.begin(); it != m_mangServer.m_socket_connections.end();)
    {
        int sd = it->second;
        if (FD_ISSET(sd, &readfds) && it->first != IDINNER)
        {
            readFromSocket(sd, it, buffer, vec_canbus);
        }
        else if (FD_ISSET(sd, &readfds) && it->first == IDINNER)
        {
            Cross_platform::cress_read(sd, buffer, 0); // Read for internal purposes
        }
        ++it;
    }
}

void ReceiveManager::readFromSocket(int sd, ItMap & it_map, char *buffer,std::vector<CanBus> &vec_canbus)
{
    int valread = Cross_platform::cress_read(sd, buffer, 0);
    if (valread <= 0)
    {
        close_socket(sd);
        it_map = m_mangServer.m_socket_connections.erase(it_map); // Remove connection if socket is closed
    }
    else if (valread > 0)
    {
        auto result = Data_manipulator::extract_id_and_data(buffer, valread);
        CanBus cb = result.value();
        writeCanMessageToLog(cb, LOGFILE);
        vec_canbus.push_back(cb);
    }
    else
    {
        LOG_ERROR("recv error");
    }
}

int ReceiveManager::performSelect(fd_set &readfds, int max_sd)
{
    int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR))
    {
        LOG_ERROR("select error");
    }
    return activity;
}

void ReceiveManager::insert_fd(fd_set &set, int &max_fd)
{
    Locker locker(m_mangServer.m_map_mutex);
    for (const auto &pair : m_mangServer.m_socket_connections)
    {
        int fd = pair.second;
        if (fd > 0)
        {
            FD_SET(fd, &set);
            if (fd > max_fd)
            {
                max_fd = fd;
            }
        }
    }
}

void ReceiveManager::reset_in_loop(fd_set &set, int &fd)
{
    FD_ZERO(&set);
    fd = 0;
}

void ReceiveManager::waitForConnection()
{
    std::unique_lock<std::mutex> unordered_map_lock(m_mangServer.m_map_mutex);

    // Wait until there's a connection to handle
    while (m_mangServer.m_socket_connections.empty())
    {
        m_connection_cv.wait(unordered_map_lock);
    }
}

void ReceiveManager::schedule()
{
    int max_sd;
    fd_set readfds;
    std::vector<CanBus> vec_canbus;
    waitForConnection();

    // Main loop
    while (true)
    {
        reset_in_loop(readfds, max_sd);
        printConnectionMap();

        // Insert file descriptors to be monitored
        insert_fd(readfds, max_sd);

        // Perform select and handle activity
        int activity = performSelect(readfds, max_sd);
        if (activity <= 0)
            continue;

        handleActivity(readfds, vec_canbus);

        insertCanBusFromvectorToQueue(vec_canbus);
    }
}

void ReceiveManager::insertCanBusFromvectorToQueue(std::vector<CanBus> &vec_canbus)
{
    Locker lock(m_mangServer.m_queue_mutex);
    for (const auto &canbus : vec_canbus)
    {
        m_mangServer.m_prioritysed_masseges_queue.push(canbus);
    }
    vec_canbus.clear();
}

void ReceiveManager::notify()
{
    m_connection_cv.notify_one();
}
