#include <gtest/gtest.h>
#include "socket.h"

class SocketTest : public ::testing::Test {
protected:
    SocketTest() : sock() {}

    void SetUp() override {
        sock.create();
    }

    Socket sock;
};

TEST_F(SocketTest, CreateSocket) {
    EXPECT_NO_THROW(sock.create());
}

TEST_F(SocketTest, BindSocket) {
    EXPECT_NO_THROW(sock.bind(8080));
}

TEST_F(SocketTest, ListenSocket) {
    sock.bind(8080);
    EXPECT_NO_THROW(sock.listen());
}

TEST_F(SocketTest, AcceptConnection) {
    sock.bind(8080);
    sock.listen();

    // Simulate a client connection in a separate process or thread
    // This is just a placeholder for actual client connection code
    std::thread client_thread([] {
        Socket client_sock;
        client_sock.create();
        client_sock.connect("127.0.0.1", 8080);
    });

    // Allow some time for client connection
    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_NO_THROW(sock.accept());

    client_thread.join(); // Ensure client thread finishes
}

TEST_F(SocketTest, SendReceiveData) {
    sock.bind(8080);
    sock.listen();

    std::thread client_thread([] {
        Socket client_sock;
        client_sock.create();
        client_sock.connect("127.0.0.1", 8080);
        const char *message = "Hello";
        client_sock.send((void *)message, strlen(message) + 1);
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    FD client_fd = sock.accept();

    char buffer[256];
    EXPECT_EQ(sock.recv(buffer, sizeof(buffer)), 6);
     std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_STREQ(buffer, "Hello");

    client_thread.join(); // Ensure client thread finishes
}

TEST_F(SocketTest, ConnectSocketWithoutAccept) {
    std::stringstream buffer;
    std::streambuf* prevCoutBuf = std::cerr.rdbuf();
    std::cerr.rdbuf(buffer.rdbuf());
    sock.connect("localhost" , 801);
    std::string line = buffer.str();
    std::cerr.rdbuf(prevCoutBuf);
    ASSERT_EQ(line.find("connect failed with errno: 111") != std::string::npos , 1);
}

// TEST_F(SocketTest, DestructorCleansUp) {
//     EXPECT_NO_THROW({
//         Socket temp_sock;
//         temp_sock.create();
//         // temp_sock.~Socket(); // Explicitly call destructor
//     });
// }
