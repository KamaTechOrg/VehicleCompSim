// Implementation of the ServerSocket class

#include "serverSocket.h"
#include "socketException.h"


ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::send(void * data , size_t size) const
{
  
  if ( ! Socket::send ( data , size ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


const ServerSocket& ServerSocket::recv(void * data) const
{
  if ( ! Socket::recv ( data ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}

void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}