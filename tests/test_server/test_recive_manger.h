// #include <gtest/gtest.h>

// #include "receive_manger.h"


// Receive_manger manger;

// TEST(TestReciveManger, TestPrintMap){
//     std::unordered_map<int, int> map;

//     std::stringstream buffer;
//     std::streambuf *prevCoutBuf = std::cout.rdbuf();
//     std::cout.rdbuf(buffer.rdbuf());
   
//     manger.print_arr(map);

//     std::string line = buffer.str();

//     // check if map is empty
//     ASSERT_EQ(line.size(),0);
//     map[0] = 11;
//     map[1] = 22;
//     map[2] = 33;
//     manger.print_arr(map);
//     line = buffer.str();
//     std::cout.rdbuf(prevCoutBuf);
//     std::cout << line << std::endl;

//     // check if map is not empty
//     ASSERT_NE(line.find("11"), std::string::npos);
//     ASSERT_NE(line.find("22"), std::string::npos);
//     ASSERT_NE(line.find("33"), std::string::npos);
  
// }

// TEST(TestReciveManger , TestSelctFunction){
//     std::priority_queue<CanBus, std::vector<CanBus>, std::greater<CanBus>> &min_heap ;
//     std::mutex heap_mutex ; 
//     std::mutex map_mutex; 
//     std::unordered_map<int, FD>  m_connections  
//     manger.select_menger(min_heap , heap_mutex , map_mutex , m_connections) ;
// }

