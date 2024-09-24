add_library(RSA src/RSA_ENC.cpp src/RSA_KEY.cpp)
target_include_directories(RSA PUBLIC include)
target_link_libraries(RSA PRIVATE HsmStatus)
target_link_libraries(RSA PRIVATE BigNum)



#testing
add_executable(RSATest test/RSA_test.cpp)
target_link_libraries(RSATest RSA GTest::gtest_main)
target_link_libraries(RSATest BigNum)
target_link_libraries(RSATest HsmStatus)


gtest_discover_tests(RSATest)
