#include "mang_req.h"

#include <iostream>
#include <cstring> // for memcpy
#include <cstdlib> // for atoi

Mamge_req::Mamge_req()
{
}

void Mamge_req::recv() {
    std::string extractedStr;

    // Extract characters until '!' is found
    for (int i = 0; i < size_recv; ++i) {
        if (buffer[i] == '!') {
            break;
        }
        extractedStr += buffer[i];
    }

    // Convert the extracted string to integer
    dest_id = std::atoi(extractedStr.c_str());
    std::cout << "IIIIIIDDDDDDDD "  << dest_id << std::endl;
}

void Mamge_req::send()
{
}
