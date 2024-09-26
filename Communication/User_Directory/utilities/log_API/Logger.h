#pragma once

#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#include <string_view>
#include <vector>
#include <ctime>
#include <chrono>

#include "Sink_interface.h"

// Enum to represent log levels


// Logger class for managing and writing log messages
class Logger {
private:
    
    inline static std::vector<std::unique_ptr<Sink>> sinks;

    // Convert log level to string
    static std::string logLevelToString(LogLevel level);

public:
    static void addSink(std::unique_ptr<Sink> sink);
    static void log(LogLevel msgLevel, const std::string& message);
};

// Macros for easier logging
#define LOG_TRACE(message) Logger::log(LogLevel::Trace, message)
#define LOG_DEBUG(message) Logger::log(LogLevel::Debug, message)
#define LOG_INFO(message) Logger::log(LogLevel::Info, message)
#define LOG_WARN(message) Logger::log(LogLevel::Warn, message)
#define LOG_ERROR(message) Logger::log(LogLevel::Error, message)
#define LOG_CRITICAL(message) Logger::log(LogLevel::Critical, message)

