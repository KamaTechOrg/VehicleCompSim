cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Create a library from the utilities
add_library(utilities STATIC
    utilities/socket.cpp
)

# Specify include directories for the utilities library
target_include_directories(utilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
)

# Define the client executable
add_executable(client
    client/src/main.cpp
)

# Link the utilities library to the client executable
target_link_libraries(client PRIVATE utilities)

# Specify include directories for the client
target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/client/include
)

# Define the server executable (if needed)
add_executable(server
    server/src/main.cpp
)

# Link the utilities library to the server executable (if server is needed)
target_link_libraries(server PRIVATE utilities)

# Specify include directories for the server (if server is needed)
target_include_directories(server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/server/include
)