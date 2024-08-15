#pragma once
#include "../../../../demo sensors/engine/library_to_user/utilities/constants.h"

class Mamge_req {
public:
    Mamge_req();
    void recv();
    void send();

    char buffer[200];
    int size_recv = 200;

    int source_id;
    int dest_id;



};
