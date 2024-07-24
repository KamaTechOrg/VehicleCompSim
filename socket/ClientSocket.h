// Definition of the ClientSocket class

#ifndef ClientSocket_class
#define ClientSocket_class

#include "socket.h"


class ClientSocket : public Socket
{
 public:

  ClientSocket ( std::string host, int port );
  virtual ~ClientSocket(){};

  const ClientSocket& send( void * data ,size_t size ,int s_id  ) const;
  const ClientSocket& recv( void * ,size_t) const;

};


#endif