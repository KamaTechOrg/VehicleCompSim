add_library(BigNum src/BigNumWithMinus.cpp src/BigNum.cpp)
target_include_directories(BigNum PUBLIC include)

# testing
add_executable(BigNumTest test/BigNum_test.cpp)
target_link_libraries(BigNumTest BigNum GTest::gtest_main)
gtest_discover_tests(BigNumTest)

add_executable(BigNumWithMinusTest test/BigNumWithMinus_test.cpp)
target_link_libraries(BigNumWithMinusTest BigNum GTest::gtest_main)
gtest_discover_tests(BigNumWithMinusTest)

