#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>  // Include this header for std::ofstream


#include "IpFileHanler.h"

// Function to check if a file exists in the TEMP folder and delete it
void clearIpFile() {
    const char* tempPath = std::getenv("TEMP");
    if (!tempPath) {
        std::cerr << "Could not retrieve TEMP folder path." << std::endl;
        return;
    }

    std::filesystem::path filePath = std::filesystem::path(tempPath) / FILE_NAME;

    // Check if the file exists
    if (std::filesystem::exists(filePath)) {
        std::cout << "File exists. Deleting: " << filePath << std::endl;

        // Attempt to delete the file
        std::error_code ec; // For error handling
        std::filesystem::remove(filePath, ec);

        if (ec) {
            std::cerr << "Error deleting file: " << ec.message() << std::endl;
        } else {
            std::cout << "File deleted successfully." << std::endl;
        }
    }
}

// Function to write a new file with a given string
void writeIpFile(const std::string& ip) {
    const char* tempPath = std::getenv("TEMP");
    if (!tempPath) {
        std::cerr << "Could not retrieve TEMP folder path." << std::endl;
        return;
    }

    std::filesystem::path filePath = std::filesystem::path(tempPath) / FILE_NAME;

    // Write to the file
    std::ofstream outFile(filePath);
    if (outFile) {
        outFile << ip;
        outFile.close();
        std::cout << "File written successfully: " << filePath << std::endl;
    } else {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
    }
}

