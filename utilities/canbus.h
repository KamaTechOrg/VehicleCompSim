#pragma once
#include <string>

class CanBus {
    
private:
    int sourceId;
    int destId;
    std::string message;
    int messageLength;
    bool rtr;
    int crc;

    
    

public:
    
    CanBus(int srcId, int dstId, std::string msg, int msgLen);

      int getSourceId() const {
        return sourceId;
    }
  
};
