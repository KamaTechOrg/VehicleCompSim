#include <iostream>
#include <thread>
#include "../../Communication/client/include/client.h"
#include "../vehicle_components_id_config.h"

using namespace std;

double speed = 0;

pair<string, string> split(string s, char del){

    pair<string, string>  res;
    int i = 0;

    while (i < s.length() && s[i] != del)
    {
        res.first += s[i++];
    }

    ++i;

    while (i < s.length())
    {
        res.second+= s[i++];
    }

    return res;
}

void pressGas(int increase) {
    // Generate a random speed increase between 1 and 10
    speed += increase;
    std::cout << "Accelerated by " << increase << ". Current speed: " << speed << " km/h" << std::endl;
}

void pressBrakes(int decrease) {
    // Generate a random speed decrease between 1 and 10, ensuring speed doesn't go negative
    speed -= decrease;
    if (speed < 0) {
        speed = 0; // Prevent speed from going negative
    }
    std::cout << "Slowed down by " << decrease << ". Current speed: " << speed << " km/h" << std::endl;
}

int main()
{
    std::cout << "wheels starting";

    ClientSocket client(vehicle_components_id_config::WHEELS);

    char buffer[MAXRECV];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        auto pair_recv = client.listen(buffer, sizeof(buffer));
        if (pair_recv.first != ListenErrorCode::SUCCESS)
        {
            client.shut_down();
            return 1;
        }

        auto messege = split(buffer, ' ');
        auto action = stoi(messege.first);
        auto amount = stoi(messege.second);

        switch (action) {
        case ACTIONS::PRESS_BREAKS:
            pressBrakes(amount);
            break;
        case ACTIONS::PRESS_GAS:
            pressGas(amount);
            break;
        default:
            break;
        }
    }
    return 0;
}
