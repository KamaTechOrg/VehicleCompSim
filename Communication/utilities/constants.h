
#pragma once

#include <string>
#include <ostream>

const int PORTSERVER = 50000;
const std::string IPSERVER = "127.0.0.1";
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 20;
const int MAXRECV = 500;
const int MAXRECVID = 50;
const int IDINNER = 55;
const std::string LOGFILE = "log.txt";

enum class ListenErrorCode
{
    SUCCESS = 0,
    SERVER_DISCONNECTED, // Server disconnected
    RECEIVE_ERROR,       // Error while receiving data
};

enum class sendErrorCode
{
    SUCCESS = 0,
    SENDFAILED, // Send failed
};

static std::ostream &operator<<(std::ostream &os, ListenErrorCode code)
{
    switch (code)
    {
    case ListenErrorCode::SUCCESS:
        os << "Success";
        break;
    case ListenErrorCode::RECEIVE_ERROR:
        os << "Receive Error";
        break;
    case ListenErrorCode::SERVER_DISCONNECTED:
        os << "Server disconnected";
        break;
    // Handle other error codes
    default:
        os << "Unknown Error";
        break;
    }
    return os;
}

static std::ostream &operator<<(std::ostream &os, sendErrorCode code)
{
    switch (code)
    {
        case sendErrorCode::SUCCESS:
        os << "SUCCESS " ;
        break;
        case sendErrorCode::SENDFAILED:
        os << "SENDFAILED " ;

        default:
        os << "Unknown error";
        break;
    }
    return os;
}

// using FD = int;
