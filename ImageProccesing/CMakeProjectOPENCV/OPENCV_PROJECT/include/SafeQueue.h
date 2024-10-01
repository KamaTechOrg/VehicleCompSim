#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>

class SafeQueue {
private:
    std::queue<std::string> queue;
    mutable std::mutex mtx;

public:
    void enqueue(const std::string& str);
        
    std::string dequeue() ;

    bool isEmpty() const ;
};
