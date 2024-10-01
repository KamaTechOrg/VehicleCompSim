#include "Manager.h"
#include "runLaneDetection.h"
#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include "client.h"

using namespace std;


bool is_valid_id(int id)
{
    return id > 0;
}

void send_thread(ClientSocket &client, int id, int d_id, SafeQueue& queue)
{

    while (true)
    {
        
		std::string h = queue.dequeue();
		if(h== "BYE BYE")break;
		h = "ID: " + std::to_string(id) + " Message: " + h;
        sendErrorCode code = client.send((void *)h.c_str(), h.size(), id, d_id);
        if (code == sendErrorCode::SENDFAILED)
        {
            std::cout << code << std::endl;
            return;
        }
       
    }
}


int main()
{    
	
    int id = 1;
	int d_id = 2;//main computer
	SafeQueue queue;

    ClientSocket client(id);

    thread t1(send_thread, ref(client), id, d_id, ref(queue));
	Manager(queue).runTracking();
 	//runLaneDetection();

	std::string end = "BYE BYE";
	queue.enqueue(end);
    t1.join();
    

    return 0;
}
