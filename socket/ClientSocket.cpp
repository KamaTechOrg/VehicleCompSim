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


const ClientSocket& ClientSocket::send(void * data ,size_t size ,int s_id) const
{
  
  if ( ! Socket::send ( data,size ,s_id ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


const ClientSocket& ClientSocket::recv(void * data ,size_t len)const
{
  if ( ! Socket::recv ( data,len ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}