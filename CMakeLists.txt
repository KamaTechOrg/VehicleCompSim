cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(client/include)
include_directories(server/include)
include_directories(utilities)

# Gather all .cpp files for client and server
file(GLOB_RECURSE CLIENT_SRC_FILES "client/src/*.cpp")
file(GLOB_RECURSE SERVER_SRC_FILES "server/src/*.cpp")
file(GLOB_RECURSE UTILITIES_SRC_FILES "utilities/*.cpp")

# Create client library
add_library(client_lib STATIC ${CLIENT_SRC_FILES} ${UTILITIES_SRC_FILES})

# Create server library
add_library(server_lib STATIC ${SERVER_SRC_FILES} ${UTILITIES_SRC_FILES})
