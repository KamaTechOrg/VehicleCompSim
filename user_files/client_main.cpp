#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <atomic>
#include "client.h"

using namespace std;



bool is_valid_id(int id) {
    return id > 0;
}

void send_thread(ClientSocket &client, int id) {
    string m;
    int d_id;

    while (true) {
        cout << "Enter your d_id: ";
        cin >> d_id;

        cout << "Enter your message: ";
        cin >> m;

        client.send((void *)m.c_str(), m.size(), id, d_id);
    }
}

void listen_thread(ClientSocket &client) {
    char buffer[MAXRECV];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        client.listen(buffer, sizeof(buffer));
        cout << "Received: " << buffer << endl;
    }
}

int main() {
    int id = 0;

    
    cout << "Enter id: ";
    cin >> id;
    if (!is_valid_id(id)) {
        throw invalid_argument("Invalid id");
    }

    ClientSocket client(id);

    thread t1(send_thread, ref(client), id);   
    thread t2(listen_thread, ref(client));     

    t1.join();
    t2.join();

    return 0;
}