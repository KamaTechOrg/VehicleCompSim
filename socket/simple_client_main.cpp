#include "ClientSocket.h"
#include "socketException.h"
#include <iostream>
#include <string>

int main ( int argc, int argv[] )
{
  try
    {

      ClientSocket client_socket ( "localhost", 30000 );

      std::string reply;
while(true){
      try
	{
    std::cout << "write meseege";
    std::string ss;
    std::cin >> ss;
	  client_socket << ss;
	  client_socket >> reply;
	}
      catch ( SocketException& ) {}

      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;

    }
  }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }
  return 0;
}