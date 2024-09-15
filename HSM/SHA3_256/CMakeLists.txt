add_library(sha256  src/SHA256.cpp)
target_include_directories(sha256 PUBLIC include)

add_executable(SHA256Test test/sha256_test.cpp)
target_link_libraries(SHA256Test sha256 GTest::gtest_main)
gtest_discover_tests(SHA256Test)