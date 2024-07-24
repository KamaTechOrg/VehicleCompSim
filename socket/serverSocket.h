// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "socket.h"


class ServerSocket : public Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){};
  virtual ~ServerSocket();

  const ServerSocket& send ( void * data ,size_t size, int s_id) const;
  const ServerSocket& recv( void * ,size_t) const;

  void accept ( ServerSocket& );

};


#endif