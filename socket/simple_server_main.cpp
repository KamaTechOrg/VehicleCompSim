#include "serverSocket.h"
#include "socketException.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../../../../../usr/include/c++/11/bits/algorithmfwd.h"

int main ( int argc, int argv[] )
{
  std::cout << "running....\n";

  try
    {
      // Create the socket
      ServerSocket server ( 30000 );

      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );
		std::cout << "ffffffff" << std::endl;
	  try
	    {
	      while ( true )
		{
		  std::string data;
		  new_sock >> data;
		  std::reverse(data.begin(), data.end());
		  new_sock << data;
		}
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}