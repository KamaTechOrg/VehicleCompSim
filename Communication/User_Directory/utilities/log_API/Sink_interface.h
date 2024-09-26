#pragma once

#include <string>

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Off
};

// Base class for log sinks (Console, File, etc.)
class Sink {
public:
    virtual ~Sink() = default;
    virtual void write(LogLevel level, const std::string& message) = 0;
};