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
    std::string s;
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        auto pair_recv = client.listen(buffer, sizeof(buffer));
        
        s += buffer;
        int count = 0;
        for(int i = 0; i <= s.size() - 3 ; i+=3){
            if (s[i] == 'a' && s[i+1] == ' ' && s[i+2] == 'c'){
                count++;
            }
        }

        std::cout << "count = " << count << std::endl;

        if (pair_recv.first != ListenErrorCode::SUCCESS)
        {
            client.shut_down();
            return;
        }
        std::cout << "Received buffer: " << buffer << std::endl;
    }
}

int main()
{
    
    // Configure logger with different sinks and levels
    Logger::addSink(std::make_unique<ConsoleSink>(LogLevel::Info));  
    Logger::addSink(std::make_unique<FileSink>("/home/mefathim-tech-41/text_file.txt", LogLevel::Info));

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