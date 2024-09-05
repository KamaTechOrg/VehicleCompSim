add_library(sha256  src/SHA-256.cpp)
target_include_directories(sha256 PUBLIC include)

add_executable(SHA256Test test/sha256_test.cpp)
