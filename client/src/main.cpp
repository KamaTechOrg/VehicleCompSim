#include <iostream>

#include "client.h"

using namespace std;

int main(){
    ClientSocket client(10);
    std::string m { "Hello"};
    client.send((void *)m.c_str() , m.size(), 10 ,100);
    char buffer[MAXRECV];
    client.listen(buffer , MAXRECV);
}