#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include "client.h"

using namespace std;

// this page is example of how to use the client socket library


bool is_valid_id(int id)
{
    return id > 0;
}

void send_thread(ClientSocket &client, int id)
{
    string m;
    int d_id;

    while (true)
    {
        cout << "Enter your d_id: ";
        cin >> d_id;
        cin.ignore();
        cout << "Enter your message: ";
        std::getline(cin, m);
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
    int id = 0;

    cout << "Enter id: ";
    cin >> id;
    cin.ignore();
    if (!is_valid_id(id))
    {
        throw invalid_argument("Invalid id");
    }

    ClientSocket client(id);

    thread t1(send_thread, ref(client), id);
    // use listen socket
    thread t2(listen_thread, ref(client));

    // use listen asynchronously
    // char buffer[MAXRECV];
    // auto func = [](ListenErrorCode){std::cout << "listen" << std::endl; };
    // client.listenAsync(buffer , sizeof(buffer),func);
    // std::string mm = buffer;

    t1.join();
    t2.join();

    

    return 0;
}