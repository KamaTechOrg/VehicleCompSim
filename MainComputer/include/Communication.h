#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

// Runs all comuunication activities with sensors and Image Processing
class Communication {
public:
    Communication();
    ~Communication();

    void listenTo(int portNumber);
    void sendTo(int portNumber, const std::string& message);
    std::string getMessageFromQueue();

    std::string sendAndReceiveLoop(const std::string& serverIP, int portNumber);
    void processServerResponse(const std::string& response);

private:
    std::queue<std::string> _messagesQueue;
    std::mutex queueMutex; // Mutex to synchronize access to the queue
    std::condition_variable messageAvailable; // Condition variable to signal new message arrival

    int createSocket();
    void initializeWinsock();
    void cleanupWinsock();
};

#endif // COMMUNICATION_H
