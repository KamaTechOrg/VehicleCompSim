
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
        return "Source ID: " + std::to_string(sourceId) + "\n" +
               "Destination ID: " + std::to_string(destId) + "\n" +
               "Message Size: " + std::to_string(messageLength) + "\n" +
               "Message: " + message + "\n";
    }



void writeCanMessageToLog(CanBus & message , const std::string & filename)
 {
    std::ofstream logFile;
    
   
    logFile.open(filename, std::ios_base::app);
    if (logFile.is_open()) {
        logFile << "time: " << Data_manipulator::getCurrentTime() <<'\n';
        logFile << message.getFormattedMessage();
        logFile << "\n ----------------------------------------------------------------\n\n";
        logFile.close();
    } else {
        std::cerr << "Error: Unable to open log file!" << std::endl;
    }
}