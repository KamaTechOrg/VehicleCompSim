#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "data_manipulator.h"

class CanBus {

public:
    // Constructor to initialize a CanBus message with source ID, destination ID, message, and message length.
    CanBus(int srcId, int dstId, std::string msg, int msgLen);

    // Overload the '>' operator for comparing CanBus messages.
    bool operator >(const CanBus& rhs) const;

    // Get the source ID of the message.
    int getSourceId() const { return sourceId; }

    // Get the destination ID of the message.
    int getDestinationId() const { return destId; }

    // Get the source ID (duplicate method, consider removing).
    int getSourcId() const { return sourceId; }

    // Get the size of the message.
    int getMessageSize() const { return messageLength; }

    // Get the message content.
    std::string getMessage() const { return message; }

    // Get the formatted message (implementation not shown).
    std::string getFormattedMessage() const;

    //get the crc value
    int getCrc() const { return m_crc; }

    // set the crc value
    void setCrc(int crc) { m_crc = crc; }
private:
    int m_crc;               // Checksum for error detection.
    int destId;            // Destination ID for the message.
    std::string message;   // Actual message content.
    int messageLength;     // Length of the message.
    int sourceId;         // Source ID of the message.

    bool rtr;             // Remote Transmission Request flag (not used in provided methods).
};

// Write a CanBus message to a log file.
void writeCanMessageToLog(CanBus & message, const std::string & filename);

// Convert a CanBus message to a log format.
std::string CanMessageToLog(CanBus & message);
