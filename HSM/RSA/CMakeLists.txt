add_library(RSA src/rsaBigNum.cpp src/RSA.cpp)
target_include_directories(RSA PUBLIC include)

#testing
add_executable(RSATest test/RSA_test.cpp)
target_link_libraries(RSATest RSA GTest::gtest_main)
gtest_discover_tests(RSATest)

add_executable(RSA_BigNumTest test/RSA_BigNum_test.cpp)
target_link_libraries(RSA_BigNumTest RSA BigNum GTest::gtest_main)
gtest_discover_tests(RSA_BigNumTest)
