

#include "socket.h"

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

Socket::Socket() : m_sock(-1)
{

  memset(&m_addr, 0, sizeof(m_addr));
}

Socket::~Socket()
{
  if (is_valid())
    ::close(m_sock);
}

void Socket::create()
{
  m_sock = socket(AF_INET, SOCK_STREAM, 0);

  if (!is_valid()){
        // throw...
  }
    

  // TIME_WAIT - argh
  int on = 1;
  if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on)) == -1){
         // throw...
  }
   

  
}

void Socket::bind(const int port)
{

  if (!is_valid()){
         // throw...
  }
    

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons(port);

  int bind_return = ::bind(m_sock, (struct sockaddr *)&m_addr, sizeof(m_addr));

  if (bind_return == -1){
     std::cout << "status == -1   errno == " << errno << "  in Socket::bind\n";
    
  }

  
}

void Socket::listen() const
{
  if (!is_valid())
  {
     // throw...
  }

  int listen_return = ::listen(m_sock, MAXCONNECTIONS);

  if (listen_return == -1)
  {
    std::cout << "status == -1   errno == " << errno << "  in Socket::listen\n";
     // throw...
  }

  
}

FD Socket::accept() 
{
  int addr_length = sizeof(m_addr);
  FD fd = ::accept(m_sock, (sockaddr *)&m_addr, (socklen_t *)&addr_length);
  
  if (fd <= 0)
  {
    std::cout << "status == -1   errno == " << errno << "  in Socket::eccept\n";
    // throw...
  }
  else
    std::cout << "connection is on " << fd << std::endl;
    return fd;
}

void Socket::send(void *data, size_t size) const
{

  int status = ::send(m_sock, data, size, MSG_NOSIGNAL);
  if (status == -1)
  {
    std::cout << "status == -1   errno == " << errno << "  in Socket::send\n";
    // throw...
  }

}

void Socket::recv(void *data, size_t len) const
{
    // Allocate buffer
    char buf[len];

    memset(buf, 0, len);

    // Receive data
    int status = ::recv(m_sock, buf, len, 0);

    if (status == -1)
    {
        std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
    }
    else if (status == 0)
    {
        // Client disconnected
    }
    else
    {
        buf[status] = '\0';
        std::cout << buf << std::flush;
        // Copy received data to the provided buffer
        memcpy(data, buf, std::min(len, static_cast<size_t>(status)));

        // Add delimiter if there's space
        if (status < static_cast<int>(len))
        {
            static_cast<char*>(data)[status] = '!';
        }
    }
}

void Socket::set_FD(int fd)
{
  m_sock = fd;
}

void Socket::connect(const std::string host, const int port)
{
  if (!is_valid())
    {
         // throw...
    }

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons(port);

  int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);

  if (errno == EAFNOSUPPORT)
    {
         std::cout << "Address not supported\n";
         // throw...
    }

  status = ::connect(m_sock, (sockaddr *)&m_addr, sizeof(m_addr));

  if (status == 0)
    {
         // std::cout << "Connected to server\n";   
    }
  else
  {
    std::cout << "status == -1   errno == " << errno << "  in Socket::connect\n";
    // throw...
  }
}
