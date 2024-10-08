#include <iostream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void logMessage(const std::string& message, const std::string& destFile) {
    std::ofstream logfile(destFile, std::ios_base::app); // Open file in append mode
    if (logfile.is_open()) {
        // Get current time
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        // Format time as [YYYY-MM-DD HH:MM:SS]
        logfile << "[" << (localTime->tm_year + 1900) << "-"
            << std::setw(2) << std::setfill('0') << (localTime->tm_mon + 1) << "-"
            << std::setw(2) << std::setfill('0') << localTime->tm_mday << " "
            << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"
            << std::setw(2) << std::setfill('0') << localTime->tm_min << ":"
            << std::setw(2) << std::setfill('0') << localTime->tm_sec << "] "
            << message << std::endl;
    }
}


int main(int argc, char* argv[]) {
    auto startTime = std::time(nullptr);
    
    std::string destFile(std::string() + argv[0] + "/logs/log" + intToString(time(0)) + ".txt");
    int i = 100;
    while (--i > 0) {
        std::string message = (argc > 1 ? argv[1] : argv[0]) + std::string(" is running ") +
            intToString(static_cast<int>(std::time(nullptr) - startTime)) + " seconds.";
        logMessage(message, destFile);
        std::cout << message << std::endl;
        Sleep(3000); // Sleep for 3 seconds
    }

    return 0;
}
