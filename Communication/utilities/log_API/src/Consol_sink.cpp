#include "Consol_sink.h"


// Constructor for ConsoleSink
ConsoleSink::ConsoleSink(LogLevel logLevel) : level(logLevel) {}

// Write log message to the console if the log level is high enough
void ConsoleSink::write(LogLevel level, const std::string& message) {
    if (level >= this->level) {
        std::cout << message << std::endl;
    }
}


