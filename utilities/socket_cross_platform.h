#pragma once

#include <unistd.h>

#include "constants.h"
#include "socket.h"


namespace Cross_platform{
    int cress_read(int sd, char *buf, int flags);
    int cress_send(FD d_s, char *buf, size_t size);
}