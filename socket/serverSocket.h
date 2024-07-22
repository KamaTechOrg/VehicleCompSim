// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "socket.h"


class ServerSocket : private Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){};
  virtual ~ServerSocket();

  const ServerSocket& send ( void * data ,size_t size) const;
  const ServerSocket& recv( void *) const;

  void accept ( ServerSocket& );

};


#endif