#include <iostream>

#include "client.h"
#include <string.h>

using namespace std;

int main()
{
    int id = 10;
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