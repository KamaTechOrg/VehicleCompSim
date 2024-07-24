#include "serverSocket.h"
#include "socketException.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../../../../../usr/include/c++/11/bits/algorithmfwd.h"
#include <string.h>

int main(int argc, int argv[])
{
	std::cout << "running....\n";

	try
	{
		// Create the socket
		ServerSocket server(3000);

		while (true)
		{

			ServerSocket new_sock;
			server.accept(new_sock);

			try
			{
				while (true)
				{
					void *buffer[MAXRECV];
					memset(buffer, 0, 10);
					new_sock.recv(buffer, MAXRECV);
					new_sock.send(buffer, MAXRECV , new_sock.socket_id());
				}
			}
			catch (SocketException &)
			{
			}
		}
	}
	catch (SocketException &e)
	{
		std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}

	return 0;
}