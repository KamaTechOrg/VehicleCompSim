#pragma once

#include <string>

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

    
private:
    int destId;
    std::string message;
    int messageLength;
    int sourceId;
  
    bool rtr;
    int crc;
};
