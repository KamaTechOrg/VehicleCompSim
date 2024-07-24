#include "ClientSocket.h"
#include "socketException.h"
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

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
        cin >> ss;
        ss += '#';
        void *data = static_cast<void *>(const_cast<char *>(ss.c_str()));
        size_t size = ss.size();

        client_socket.send(data, size, client_socket.socket_id());

        void *buffer[MAXRECV];
        memset(buffer, 0, MAXRECV);
        client_socket.recv(buffer, MAXRECV);

        cout << "we received form server:";
        int i = 0;
        while (*(char *)(buffer + i) != '#' && i < MAXRECV)
        {
          cout << *((char *)(buffer) + i);
          i++;
        }
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