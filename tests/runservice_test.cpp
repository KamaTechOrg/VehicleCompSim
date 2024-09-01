// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "../src/services/runservice.h"
// #include "../src/gui/customscene.h"

// using ::testing::_;
// using ::testing::Return;
// using ::testing::Invoke;

// class MockCustomScene : public CustomScene {
// public:
//     MOCK_METHOD(QList<QGraphicsItem*>, items, (), (const, override));
// };

// class RunServiceTest : public ::testing::Test {
// protected:
//     void SetUp() override {
//         runService.setScene(&mockScene);
//     }

//     MockCustomScene mockScene;
//     RunService runService;
// };

// TEST_F(RunServiceTest, Constructor) {
//     EXPECT_EQ(runService.isRunning, false);
//     EXPECT_EQ(runService.timer, 0);
// }

// TEST_F(RunServiceTest, SetScene) {
//     CustomScene scene;
//     runService.setScene(&scene);
//     // Assuming there's a way to get the scene from RunService, which is not shown in the provided code
//     // EXPECT_EQ(runService.getScene(), &scene);
// }

// TEST_F(RunServiceTest, StartWithoutOnFinish) {
//     EXPECT_CALL(mockScene, items())
//         .WillOnce(Return(QList<QGraphicsItem*>()));

//     runService.start();
//     EXPECT_EQ(runService.isRunning, true);
// }

// TEST_F(RunServiceTest, StartWithOnFinish) {
//     EXPECT_CALL(mockScene, items())
//         .WillOnce(Return(QList<QGraphicsItem*>()));

//     bool onFinishCalled = false;
//     runService.start([&onFinishCalled]() {
//         onFinishCalled = true;
//     });

//     EXPECT_EQ(runService.isRunning, true);
//     runService.stop();
//     EXPECT_EQ(onFinishCalled, true);
// }

// TEST_F(RunServiceTest, Stop) {
//     runService.start();
//     runService.stop();
//     EXPECT_EQ(runService.isRunning, false);
// }

// TEST_F(RunServiceTest, SetTimer) {
//     runService.setTimer(10);
//     EXPECT_EQ(runService.timer, 10);
// }

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

#include <gtest/gtest.h>
#include <gmock/gmock.h>

// A simple test case to verify Google Test installation
TEST(SimpleTest, BasicAssertions) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}