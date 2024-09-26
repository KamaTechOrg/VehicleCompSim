
#pragma once

#include "Logger.h"
#include <fstream>

// File sink for logging to a file
class FileSink : public Sink {
private:
    std::ofstream file;
    LogLevel level;

public:
    FileSink(const std::string& filename, LogLevel logLevel);
    ~FileSink() override;
    void write(LogLevel level, const std::string& message) override;
    
};


