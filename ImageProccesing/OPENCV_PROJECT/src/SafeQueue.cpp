#include "SafeQueue.h"

void SafeQueue::enqueue(const std::string& str) {
	std::lock_guard<std::mutex> lock(mtx);
	queue.push(str);
}

std::string SafeQueue::dequeue() {
	std::unique_lock<std::mutex> lock(mtx);
	
	while (queue.empty()) {
		lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		lock.lock();
	}

	std::string front = queue.front();
	queue.pop();
	return front;
}

bool SafeQueue::isEmpty() const {
	std::lock_guard<std::mutex> lock(mtx);
	return queue.empty();
}