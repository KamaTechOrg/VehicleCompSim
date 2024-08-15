
// exemple for use with int main client

 #include <iostream>
#include "../../../../gui and comuncation/Comunication/demo sensors/mid computer/library_to_user/client/client.h"
#include <string.h>
#include <random>
#include <string>
#include <thread>
#include <chrono>

 using namespace std;

 bool is_valid_id(int id) {
     return id > 0;
 }

 int main()
 {
     std::random_device rd;
     std::mt19937 gen(rd());
     // Define the range
     std::uniform_int_distribution<> distr(0, 100);

     // Generate a random number in the range [0, 100]
     int random_number = distr(gen);

     int id = 15;
     ClientSocket client(id);
     std::string m;
     char buffer[200];
     int d_id = 30;
     while (true)
     {

         client.listen(buffer, sizeof(buffer));
         std::cout << buffer << std::endl;
         memset(buffer, 0, sizeof(buffer));

         std::this_thread::sleep_for(std::chrono::seconds(5));
         int random_number = distr(gen);
         m = "engine heat  = " + std::to_string(random_number);;

         client.send((void *)m.c_str(), m.size(), id, d_id);
     }
 }