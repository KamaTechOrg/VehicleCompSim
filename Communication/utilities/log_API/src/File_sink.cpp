#include "File_sink.h"

// Constructor for FileSink, opens file in append mode
FileSink::FileSink(const std::string& filename, LogLevel logLevel) : level(logLevel) {
    file.open(filename, std::ios::app); // Open in append mode
    if (!file.is_open()) {
        throw std::runtime_error("Could not open log file.");
    }
}

// Destructor for FileSink, closes the file if open
FileSink::~FileSink() {
    if (file.is_open()) {
        file.close();
    }
}

// Write log message to the file if the log level is high enough
void FileSink::write(LogLevel level, const std::string& message) {
    if (level >= this->level) {
        file << message << std::endl;
    }
}

