#include "mang_connect.h"
#include <iostream>
#include <string.h>
#include <unistd.h> // For close()
#include <cstring> // for memcpy
#include <cstdlib> 
#include <vector>
#include <utility>
static void add_fd_to_vect(std::vector<int> &clientSocket, FD new_socket)
{
    clientSocket.push_back(new_socket);
  
}

void Mange_connect::add_socket(FD new_socket)
{   
    auto pair = create(new_socket);
    

    std::cout << "the fd is : " << new_socket << std::endl;

    std::unique_lock<std::mutex> lock(m_vec_client_mutex);
    add_fd_to_vect(m_client_fd, new_socket);
    lock.unlock();
    add_to_map(pair);

}

std::pair<int, std::shared_ptr<Socket>> Mange_connect::create(FD fd)
{
    auto new_socket = std::make_shared<Socket>();
    new_socket->set_FD(fd);

    char data[MAXRECVID];

    new_socket->recv(data, MAXRECVID);

    std::cout << "ggggg: " << data << std::endl;

    new_socket->send(data, MAXRECVID);

    int id = extractid(data);

    

    return std::make_pair(id , new_socket);


  

}

void Mange_connect::add_to_map(std::pair<int, std::shared_ptr<Socket>> pair)
{
    m_connections[pair.first] = pair.second;

   
}

int Mange_connect::get_sock(int id)
{
    for(auto i : m_connections){
        std::cout << i.first << " innmapppp " << std::endl;
    }
    return m_connections[id]->get_FD();
    
}

void Mange_connect::select_menger()
{
    int newSocket, maxSd, activity, sd, valread;
    fd_set readfds;
    char buffer[MAXRECV];

      
    
    

    while (true)
    {
        std::cout << "i fi " << std::endl;
        sleep(1);
        if (!m_client_fd.empty())
        {   
            for(auto i : m_client_fd){
                std::cout << i << "ffffffffgggggggggg" << std::endl;
            }
            memset(buffer, 0, sizeof(buffer));
            FD_ZERO(&readfds);

            maxSd = 0;

            std::unique_lock<std::mutex> lock(m_vec_client_mutex);
            for (int i = 0; i < m_client_fd.size(); ++i)
            {
                sd = m_client_fd[i];
                if (sd > 0)
                    FD_SET(sd, &readfds);
                if (sd > maxSd)
                    maxSd = sd;
            }
            lock.unlock();
            std::cout << "i cum " << std::endl;
            activity = select(maxSd + 1, &readfds, NULL, NULL, NULL);
              std::cout << "i whit " << std::endl;
            if ((activity < 0) && (errno != EINTR))
            {
                perror("Select error");
                break;
            }
            

            lock.lock();
            for (int i = 0; i < m_client_fd.size(); ++i)
            {
                sd = m_client_fd[i];
                if (FD_ISSET(sd, &readfds))
                {
                
                  
                    valread = recv(sd, buffer , m_req.size_recv , 0);
                    memcpy(m_req.buffer, buffer, valread);
                    m_req.recv();

                    
                    if (valread == 0)
                    {
                        // Connection closed
                        // getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                        // std::cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) << ", port " << ntohs(address.sin_port) << std::endl;

                        close(sd);
                        
                        m_client_fd.erase(m_client_fd.begin() + i);
                        
                    
                    }
                    else if (valread > 0)
                    {
                        // Received data
                        buffer[valread] = '\0';
                        std::cout << "Received: " << buffer << std::endl;
                        if(m_connections.find(m_req.dest_id) != m_connections.end()){
                            int dest = get_sock(m_req.dest_id);
                            send(dest, m_req.buffer, valread , 0);  
                        }
                        
                    }
                    else
                    {
                        perror("recv error");
                    }
                }
            }
            lock.unlock();
        }
       
    }
}

int Mange_connect::extractid(char* data)
{
     std::string extractedStr;

    // Extract characters until '!' is found
    for (int i = 0; i < 10; ++i) {
        if (data[i] == '!') {
            break;
        }
        extractedStr += data[i];
    }

    // Convert the extracted string to integer
    int dest_id = std::atoi(extractedStr.c_str());
    return dest_id;
}



std::vector<std::pair<int, char*>> Mange_connect::extractid_and_data( char* data, int len) {
    std::vector<std::pair<int, char*>> result;
    std::string datatosend;
    int sourceid = 0;
    int destid = 0;
    int identify = 0;

    for (int i = 0; i < len; ++i) {
        if (data[i] != '!') {
            datatosend += data[i];
        } else {
           

            if (identify == 0) {
                if (!datatosend.empty()) {
                    std::cout << datatosend << std::endl;
                    sourceid = std::stoi(datatosend);
                }
                datatosend.clear();
            }
            if (identify == 1) {
                if (!datatosend.empty()) {
                    std::cout << datatosend << std::endl;
                    destid = std::stoi(datatosend);
                }
                datatosend.clear();
            }
            if (identify == 2) {
                if (!datatosend.empty()) {
                    char* dataCopy = new char[datatosend.size() + 1];
                    std::strcpy(dataCopy, datatosend.c_str());
                    result.emplace_back(destid, dataCopy);
                }
                datatosend.clear();
            }

             if (identify == 2) { identify = 0; }
            else { identify += 1; }
            
        }
    }


    return result;
}