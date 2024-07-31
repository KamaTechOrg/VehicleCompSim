cmake_minimum_required(VERSION 3.15)
project(hsm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(aes)
add_subdirectory(BigNum)
add_subdirectory(rsa)
add_subdirectory(test)


add_executable(main main.cpp)

# Link the BigNum and RSA libraries with the main executable
target_link_libraries(main PRIVATE BigNum RSA)