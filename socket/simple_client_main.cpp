#include "ClientSocket.h"
#include "socketException.h"
#include <iostream>
#include <string>

int main(int argc, int argv[])
{
  try
  {

    ClientSocket client_socket("localhost", 3000);

    while (true)
    {
      try
      {
       
        std::cout << "Write message: ";
        std::string ss;
        std::getline(std::cin, ss);

        
        void* data = static_cast<void*>(const_cast<char*>(ss.c_str()));
        size_t size = ss.size();

        
        client_socket.send(data, size);

       
        void* buffer[1024]; 
        client_socket.recv(buffer);
        
        
        std::string reply((char *)buffer, 1024);

        // Display the response from the server
        std::cout << "We received this response from the server:\n\"" << reply << "\"\n";
      }
      catch (SocketException &e)
      {
        std::cerr << "SocketException was caught: " << e.description() << "\n";
      }
    }
  }
  catch (SocketException &e)
  {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }
  return 0;
}