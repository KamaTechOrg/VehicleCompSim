#include "Logger.h"

// Helper function to convert log level to string
std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Trace: return "TRACE";
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warn: return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

// Add a sink to the logger
void Logger::addSink(std::unique_ptr<Sink> sink) {
    sinks.push_back(std::move(sink));  // Store the unique_ptr
}

// Log a message with the specified log level
void Logger::log(LogLevel msgLevel, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&now_c);
    timeStr.pop_back(); // Remove newline

    std::string logMessage = "[" + timeStr + "] [" + logLevelToString(msgLevel) + "] " + message;

    // Send the log message to each sink, if it meets the sink's log level
    for (const auto& sink : sinks) {
            sink->write(msgLevel, logMessage);
        
    }
}

// example of use the api

//   LOG_INFO("Application started");
//   LOG_DEBUG("This is a debug message");
//   LOG_ERROR("An error occurred!");