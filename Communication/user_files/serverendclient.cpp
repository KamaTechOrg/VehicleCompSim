#include <iostream>


#include "manger.h"

using namespace std;



   




#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <atomic>
#include "client.h"

using namespace std;

bool is_valid_id(int id)
{
    return id > 0;
}

void send_thread(ClientSocket &client, int id)
{
    string m;
    int d_id;
    std::vector<std::string> string_vector(100);

    // Fill the vector with values
    for (size_t i = 0; i < string_vector.size(); ++i) {
        string_vector[i] = "String" ;
    }
    int i = 0;
    cout << "start " << endl;
    cin >> d_id;
    while (i < string_vector.size())
    {
        m = string_vector[i];
        // cout << "Enter your d_id: ";
        // cin >> d_id;
        // cin.ignore();
        // cout << "Enter your message: ";
        // std::getline(cin, m);
       
        sendErrorCode code = client.send((void *)m.c_str(), m.size(), id, d_id);
        if (code == sendErrorCode::SENDFAILED)
        {
            std::cout << code << std::endl;
            return;
        }
       
    }
}

void listen_thread(ClientSocket &client)
{
    char buffer[MAXRECV];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        auto pair_recv = client.listen(buffer, sizeof(buffer));
        if (pair_recv.first != ListenErrorCode::SUCCESS)
        {
            client.shut_down();
            return;
        }
    }
}

int main()
{
    std::thread([](){
        MangServer ms;
        ms.init();
    }).detach();
   

    this_thread::sleep_for(std::chrono::seconds(1));

    int id = 0;

    cout << "Enter id: ";
    cin >> id;
    cin.ignore();
 

    ClientSocket client(id);

    thread t1(send_thread, ref(client), id);
    thread t2(listen_thread, ref(client));

    t1.join();
    t2.join();

    return 0;
}