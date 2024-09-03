#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <string>

class Communication {
public:
    std::string listenTo(int portNumber);
    void sendTo(int portNumber, const std::string& message);
    void connectTo(int portNumber);

private:
    int createSocket();
    void initializeWinsock();
    void cleanupWinsock();
};

#endif // COMMUNICATION_H
