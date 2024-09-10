#include <gtest/gtest.h>
#include "client.h"
#include "manger.h"

class ClientSocketTest : public ::testing::Test {
protected:
    ClientSocket* client;
    MangServer * server;

    void SetUp() override {
        server = new MangServer();
        server->init();
        client = new ClientSocket(1);
    }

    void TearDown() override {
        delete server;
        delete client;
    }
};

TEST_F(ClientSocketTest, clientCtor) {
    // (client->send(nullptr));  
}
