#pragma once
#include <string>

class CanBus {

public:
    
    CanBus(int srcId, int dstId, std::string msg, int msgLen);

      int getSourceId() const {
        return sourceId;
    }
    int destId;
    std::string message;
    int messageLength;
    int sourceId;
  
private:
    bool rtr;
    int crc;
};
