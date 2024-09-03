#include "canbus.h"
#include <string>



CanBus::CanBus(int srcId, int dstId, std::string msg, int msgLen) :
sourceId(srcId), destId(dstId), message(msg), messageLength(msgLen) {}




