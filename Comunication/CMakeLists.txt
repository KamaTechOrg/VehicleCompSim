# cmake example how to use this library


cmake_minimum_required(VERSION 3.10)


project(Can_Bus_Library)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the path to your library
link_directories(library_to_user/lib)

# Include directories for your library headers
include_directories(library_to_user/server)
include_directories(library_to_user/utilities)

# Create the server executable
add_executable(server_exec server_main.cpp)
target_link_libraries(server_exec client_lib server_lib)
