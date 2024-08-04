cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(client/include)
include_directories(server/include)
include_directories(utilities)

# Utilities source
set(UTILITIES_SRC
    utilities/socket.cpp
)

# Client executable
add_executable(client
    client/src/client.cpp
    client/src/main.cpp
    ${UTILITIES_SRC}
)

# Server executable
add_executable(server
    server/src/main.cpp
    server/src/mang_connect.cpp
    server/src/mang_req.cpp
    server/src/manger.cpp
    server/src/server.cpp
    ${UTILITIES_SRC})
