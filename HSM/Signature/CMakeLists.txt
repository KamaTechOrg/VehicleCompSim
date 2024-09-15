add_library(Signature src/Signature.cpp)
target_include_directories(Signature PUBLIC include)
target_link_libraries(Signature SHA256)
target_link_libraries(Signature RSA)


# add_executable(SHA256Test test/sha256_test.cpp)
# target_link_libraries(SHA256Test sha256 GTest::gtest_main)
# gtest_discover_tests(SHA256Test)