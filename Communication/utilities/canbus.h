#pragma once

#include <string>
#include <string>
#include <fstream>
#include <iostream>

#include "data_manipulator.h"

class CanBus {

public:
    
    CanBus(int srcId, int dstId, std::string msg, int msgLen);
    bool operator >(const CanBus& rhs) const ;
    int getSourceId() const {return sourceId;}


    int getDestinationId() const {
        return destId;
    }

     int getSourcId() const {
        return sourceId;
    }

    int getMessageSize() const {
        return messageLength;
    }

    std::string getMessage() const {
        return message;
    }
    std::string getFormattedMessage() const;

    int crc;
    
    
private:
    int destId;
    std::string message;
    int messageLength;
    int sourceId;
  
    bool rtr;
    
};

void writeCanMessageToLog(CanBus & message , const std::string & filename);
std::string CanMessageToLog(CanBus &message);
