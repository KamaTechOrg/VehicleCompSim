#pragma once

#include "Logger.h"

// Console sink for logging to the console
class ConsoleSink : public Sink {
private:
    LogLevel level;

public:
    ConsoleSink(LogLevel logLevel);
    void write(LogLevel level, const std::string& message) override;

};


