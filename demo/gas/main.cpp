#include <iostream>
#include <thread>
#include "../../Communication/client/include/client.h"
#include "../vehicle_components_id_config.h"

using namespace std;

int main()
{
    while (time(0) % 4 >= 2) this_thread::sleep_for(chrono::milliseconds(100));

    std::cout << "gas starting";

    ClientSocket client(vehicle_components_id_config::GAS);

    std::srand(std::time(0));
    string m;
    int d_id = vehicle_components_id_config::WHEELS;

    int amount;

    while (true)
    {
        amount = rand() % 10 + 1;
        m =  to_string(ACTIONS::PRESS_GAS) + " " + to_string(amount);

        sendErrorCode code = client.send((void *)m.c_str(), m.size(), vehicle_components_id_config::GAS, d_id);

        if (code == sendErrorCode::SENDFAILED)
        {
            std::cout << code << std::endl;
            return 1;
        }

        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}
