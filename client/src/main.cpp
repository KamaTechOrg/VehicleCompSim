#include <iostream>

#include "client.h"
#include <string.h>

using namespace std;

bool is_valid_id(int id) {
    return id > 0;
}

int main()
{
    int id = 0;
    std::cout << "enter id" << std::endl;
    cin >> id;
    if (!is_valid_id(id)){
        throw("Invalid id");
    }
    ClientSocket client(id);
    std::string m;
    char buffer[MAXRECV];

    while (true)
    {

        client.listen(buffer, sizeof(buffer));
        std::cout << buffer << std::endl;
        memset(buffer, 0, sizeof(buffer));
        cin >> m;
        client.send((void *)m.c_str(), m.size(), id, 100);
    }
}



