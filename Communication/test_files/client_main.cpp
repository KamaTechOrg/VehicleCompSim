#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <atomic>

#include "Logger.h"
#include "Console_sink.h"
#include "File_sink.h"
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
        buffer[pair_recv.second] = '\0';
        std::cout << buffer << std::endl;
    }
}

int main()
{
    
    // Configure logger with different sinks and levels
    Logger::addSink(std::make_unique<ConsoleSink>(LogLevel::Error));  
    // Logger::addSink(std::make_unique<FileSink>("/build/app.log", LogLevel::Info));

    LOG_INFO("Application started");

    

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