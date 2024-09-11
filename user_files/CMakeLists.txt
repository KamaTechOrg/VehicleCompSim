cmake_minimum_required(VERSION 3.10)

# Project name
project(UserProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the path to your library
link_directories(../build)

# Include directories for your library headers
include_directories(../client/include)
include_directories(../server/include)
include_directories(../utilities)


# Create the server executable
add_executable(server_exec server_main.cpp)
if(WIN32)
target_link_libraries(server_exec client_lib server_lib Ws2_32)
else()
target_link_libraries(server_exec client_lib server_lib )
endif()

# Create the client executable
add_executable(client_exec client_main.cpp)
if(WIN32)
target_link_libraries(client_exec client_lib server_lib Ws2_32)
else()
target_link_libraries(client_exec client_lib server_lib )
endif()
