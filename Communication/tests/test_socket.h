#include <gtest/gtest.h>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "socket.h"

class SocketTest : public ::testing::Test
{
protected:
    SocketTest() : sock() {}

    void SetUp() override
    {
        sock.create();
    }

    Socket sock;
};

TEST_F(SocketTest, CreateSocket)
{
    EXPECT_NO_THROW(sock.create());
}

TEST_F(SocketTest, BindSocket)
{
    EXPECT_NO_THROW(sock.bind(8080));
}

TEST_F(SocketTest, ListenSocket)
{
    sock.bind(8080);
    EXPECT_NO_THROW(sock.listen());
}

TEST_F(SocketTest, AcceptConnection)
{
    sock.bind(8080);
    sock.listen();

    // Simulate a client connection in a separate process or thread
    // This is just a placeholder for actual client connection code
    std::thread client_thread([]
                              {
        Socket client_sock;
        client_sock.create();
        client_sock.connect("127.0.0.1", 8080); });

    // Allow some time for client connection
    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_NO_THROW(sock.accept());

    client_thread.join(); // Ensure client thread finishes
}

TEST_F(SocketTest, SendReceiveData)
{
    std::mutex mtx;
    std::condition_variable cv;
    bool data_ready = false;
    std::string message{"Hello world!"};
    sock.bind(8080);
    sock.listen();

    std::thread client_thread([&]
                              {
        Socket client;
        std::unique_lock <std::mutex> lock(mtx);
        client.create();
        client.connect("127.0.0.1", 8080);
        cv.wait(lock,[&data_ready]{return data_ready == true;});
        client.send((char *)message.c_str(), message.size()); });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::unique_lock<std::mutex> lock(mtx);
    FD new_fd = sock.accept();
    data_ready = true;
    cv.notify_one();
    lock.unlock();
    char buffer[256] = {0};
    Socket new_server;
    new_server.set_FD(new_fd);
    ASSERT_EQ(new_server.recv(buffer, sizeof(buffer)), message.size());
    buffer[message.size()] = '\0';
    ASSERT_STREQ(buffer, "Hello world!");

    client_thread.join();
}

TEST_F(SocketTest, ConnectSocketWithoutAccept)
{
    std::stringstream buffer;
    std::streambuf *prevCoutBuf = std::cerr.rdbuf();
    std::cerr.rdbuf(buffer.rdbuf());
    sock.connect("localhost", 801);
    std::string line = buffer.str();
    std::cerr.rdbuf(prevCoutBuf);
    ASSERT_EQ(line.find("connect failed with errno: 111") != std::string::npos, 1);
}

TEST_F(SocketTest, DestructorCleansUp)
{
    EXPECT_NO_THROW({
        Socket temp_sock;
        temp_sock.create();
        // temp_sock.~Socket(); // Explicitly call destructor
    });
}
