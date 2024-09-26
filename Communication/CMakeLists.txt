cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(SYSTEM_DEPENDENT_LIBRARIES Ws2_32)

# Include directories
include_directories(client/include)
include_directories(server/include)
include_directories(tests/test_server)
include_directories(utilities)
include_directories(utilities/log_API/include)

# Gather all .cpp files for client, server, utilities, and tests
file(GLOB_RECURSE CLIENT_SRC_FILES "client/src/*.cpp")
file(GLOB_RECURSE SERVER_SRC_FILES "server/src/*.cpp")
file(GLOB_RECURSE UTILITIES_SRC_FILES "utilities/*.cpp" "utilities/log_API/src/*.cpp")
file(GLOB_RECURSE TESTS_SRC_FILES "tests/*.cpp")

# Create client library
add_library(client_lib STATIC ${CLIENT_SRC_FILES} ${UTILITIES_SRC_FILES})

# Create server library
add_library(server_lib STATIC ${SERVER_SRC_FILES} ${UTILITIES_SRC_FILES})

target_link_libraries(client_lib PRIVATE Ws2_32)
target_link_libraries(server_lib PRIVATE Ws2_32)

# Find GoogleTest
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

# Create test executable
#add_executable(run_tests ${TESTS_SRC_FILES})

# Link test executable with the libraries and GoogleTest
#target_link_libraries(run_tests client_lib server_lib ${GTEST_LIBRARIES} pthread)
