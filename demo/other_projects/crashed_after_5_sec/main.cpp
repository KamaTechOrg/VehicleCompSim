#include <iostream>
#include <chrono>
#include <thread>
char arr[80000000];
int main() {
    std::cout << "Program will start in 5 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Program started!" << std::endl;
    char* c = nullptr;
    *c = 'p';
    return 0;
}
