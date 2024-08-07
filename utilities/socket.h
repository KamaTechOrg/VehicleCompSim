// Implementation of the Socket class.
// Definition of the Socket class

#pragma once


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include "constants.h"




class Socket
{
public:
  Socket();
  ~Socket();

 
  void create();
  void bind ( const int port );
  void listen() const;
  FD accept (  ) ;

  
  void connect ( const std::string host, const int port );

  void send ( void * data ,size_t size ) const;
  void recv ( void *data, size_t len ) const;


  void set_FD( int fd );

  int get_FD() const { return m_sock; }

  bool is_valid() const { return m_sock != -1; }
 

private:
  
  int m_sock;
  sockaddr_in m_addr;
  


};