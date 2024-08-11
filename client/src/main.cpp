// #include <iostream>

// #include "client.h"
// #include <string.h>

// using namespace std;

// bool is_valid_id(int id) {
//     return id > 0;
// }

// int main()
// {
//     int id = 0;
//     std::cout << "enter id" << std::endl;
//     cin >> id;
//     if (!is_valid_id(id)){
//         throw("Invalid id");
//     }
//     ClientSocket client(id);
//     std::string m;
//     char buffer[MAXRECV];
//     int d_id;
//     while (true)
//     {

//         client.listen(buffer, sizeof(buffer));
//         std::cout << buffer << std::endl;
//         memset(buffer, 0, sizeof(buffer));
//         std::cout << "enter your d_id" << std::endl;
//         cin >> d_id;
//         std::cout << "enter your buffer" << std::endl;
//         cin >> m;
//         client.send((void *)m.c_str(), m.size(), id, d_id);
//     }
// }



