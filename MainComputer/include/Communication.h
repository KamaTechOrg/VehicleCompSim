#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

class Communication {
public:
    std::string listenTo(int portNumber);
    void sendTo(int portNumber, const std::string& message);
    void connectToSensor(int portNumber);
    std::string getMessageFromQueue();

private:
    std::queue<std::string> _messagesQueue;

    std::mutex queueMutex; // Mutex to synchronize access to the queue
    std::condition_variable messageAvailable; // Condition variable to signal new message arrival

    int createSocket();
    void initializeWinsock();
    void cleanupWinsock();
};

#endif // COMMUNICATION_H
