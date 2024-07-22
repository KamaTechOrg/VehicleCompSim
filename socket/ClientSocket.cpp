// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "socketException.h"


ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

}


const ClientSocket& ClientSocket::send(void * data ,size_t size) const
{
  if ( ! Socket::send ( data,size ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


const ClientSocket& ClientSocket::recv(void * data)const
{
  if ( ! Socket::recv ( data ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}