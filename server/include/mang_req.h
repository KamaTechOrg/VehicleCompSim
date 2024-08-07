#pragma once
#include "constants.h"

class Mamge_req {
public:
    Mamge_req();
    void recv();
    void send();

    char buffer[MAXRECV];
    int size_recv = MAXRECV;

    int source_id;
    int dest_id;



};