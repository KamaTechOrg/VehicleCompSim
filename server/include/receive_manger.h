#include <mutex>
#include <vector>
#include <memory>
#include <map>

#include "socket.h"

class Receive_manger {
public:
    
    int add_socket(int new_socket);
    std::shared_ptr<Socket> get_sock(int id);
    void select_menger();
   

private:
    
    void print_arr();
    std::pair<int, std::shared_ptr<Socket>> create(int fd);
    void add_to_map(std::pair<int, std::shared_ptr<Socket>> pair);

    std::vector<int> m_client_fd;
    std::map<int, std::shared_ptr<Socket>> m_connections;
    std::mutex m_vec_client_mutex;  
    std::mutex m_map_mutex;   
     
};
