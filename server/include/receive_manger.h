#include <mutex>
#include <vector>
#include <memory>
#include <map>
#include "compercanbus.h"

#include <queue>
#include "socket.h"

class Receive_manger {
public:
    
    int add_socket(int new_socket);
    void select_menger(std::priority_queue<CanBus, std::vector<CanBus> , CompareCanBus>& min_heap);
   

private:
    
    std::pair<int, FD> create_kay_value_id(int fd);
    FD get_sock(int id);
    void print_arr();
   
    std::map<int, FD> m_connections; 
    std::mutex m_map_mutex;   
     
};
