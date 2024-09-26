#ifndef IPFILEHANLER_H
#define IPFILEHANLER_H

#include <string>

// Define the file name as a macro
#define FILE_NAME "VehicleCompSim_comServerIp"

// Function to check if a file exists in the TEMP folder and delete it
void clearIpFile();

// Function to write a new file with a given string
void writeIpFile(const std::string& ip);


#endif // IPFILEHANLER_H
