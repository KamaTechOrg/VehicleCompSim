add_library(SHA_API src/SHA_API.cpp)
target_include_directories(SHA_API PUBLIC include)
target_link_libraries(SHA_API PRIVATE HSM_Enums)



#testing
# add_executable(RSATest test/RSA_test.cpp)
# target_link_libraries(RSATest RSA GTest::gtest_main)
# target_link_libraries(RSATest BigNum)
# target_link_libraries(SHA_API HSM_Enums)


# gtest_discover_tests(RSATest)
