# API
add_library(HSM src/HSM.cpp src/KeyStorage.cpp src/ID.cpp)
target_include_directories(HSM PUBLIC include)
target_link_libraries(HSM AES_API RSA SHA_API HSM_Enums)


#testing
add_executable(HSM_TEST test/HSM_test.cpp)
target_link_libraries(HSM_TEST HSM AES_API GTest::gtest_main)

gtest_discover_tests(HSM_TEST)


