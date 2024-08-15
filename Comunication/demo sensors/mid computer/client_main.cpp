#include <iostream>
#include "../../../../gui and comuncation/Comunication/demo sensors/mid computer/library_to_user/client/client.h"
#include <string.h>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

using namespace std;

void print_messages(vector<string>& messages, mutex& mtx) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        mtx.lock();
        for (const auto& msg : messages) {
            std::cout << msg << std::endl;
        }
        messages.clear();
        mtx.unlock();
    }
}

int main()
{
    int id = 30;
    ClientSocket client(id);
    char buffer[200];
    std::vector<std::string> messages;
    std::mutex mtx;

    // Start a thread to print messages every 5 seconds
    std::thread printer_thread(print_messages, std::ref(messages), std::ref(mtx));

    while (true)
    {
        client.listen(buffer, sizeof(buffer));
        std::cout << buffer << std::endl;

        mtx.lock();
        messages.push_back(buffer);
        mtx.unlock();

        memset(buffer, 0, sizeof(buffer));


        // Join the printer thread before exiting
        printer_thread.join();

        return 0;
    }
}
