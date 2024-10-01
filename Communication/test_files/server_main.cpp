#include <iostream>


#include "manger.h"
#include <Logger.h>
#include <Console_sink.h>

using namespace std;

int main(){

    Logger::addSink(std::make_unique<ConsoleSink>(LogLevel::Info));  // Console logs only Error and above

    LOG_INFO("Server started");

    MangServer ms;
    ms.init();

    return 0;
}