
#include <cstdint>
#include "Logger.h"
#include "canbus.h"

CanBus::CanBus(int srcId, int dstId, std::string msg, int msgLen)
 : sourceId(srcId)
 , destId(dstId)
 , message(msg)
 , messageLength(msgLen) 
 {}

bool CanBus::operator>(const CanBus &rhs) const
{
    return getSourceId() > rhs.getSourceId();
}

std::string CanBus::getFormattedMessage() const {
        return "," + std::to_string(sourceId) + 
               "," + std::to_string(destId) + 
               "," + std::to_string(messageLength) + 
               "," + message + "\n";
    }






void writeCanMessageToLog(CanBus & message , const std::string & filename)
 {
    std::ofstream logFile;
    
   
    logFile.open(filename, std::ios_base::app);
    if (logFile.is_open()) {
        logFile << Data_manipulator::getCurrentTime();
        logFile << message.getFormattedMessage();
        logFile.close();
    } else {
        LOG_ERROR("Unable to open log file!");
    }
}


std::string CanMessageToLog(CanBus &message)
{
    std::ostringstream oss;

    oss << Data_manipulator::getCurrentTime();
    oss << message.getFormattedMessage();

    return oss.str();  // Convert ostringstream to string
}
