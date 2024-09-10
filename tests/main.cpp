#include "test_client.h"
#include "test_socket.h"
#include "test_recive_manger.h"


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}