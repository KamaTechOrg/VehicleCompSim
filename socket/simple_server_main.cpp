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
      ServerSocket server ( 3000 );

      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );
		
	  try
	    {
	      while ( true )
		{
		  void * data;
		  size_t size = 1024;
		  new_sock.recv(data);
		
		  new_sock.send(data , size);
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