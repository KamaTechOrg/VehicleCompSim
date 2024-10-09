#include "socket_cross_platform.h"

#include <iostream>


int  Cross_platform::cress_read(int sd, char *buf, int flags)
{
#ifdef _WIN32
    return ::recv(sd, buf, MAXRECV, 0);
#else
    return ::recv(sd, buf, MAXRECV, 0);
#endif
}

int Cross_platform::cress_send(FD d_s,const char *buf, size_t size)
{
#ifdef _WIN32
    return ::send(d_s, static_cast<const char *>(buf), static_cast<int>(size), 0);
#else

    int status = ::send(d_s, buf, size , MSG_NOSIGNAL);
    
    return status;
#endif
}
