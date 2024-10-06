cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(services STATIC
    runservice.cpp

    runner.h
    runmanager.h runmanager.cpp
    sensorrunner.h sensorrunner.cpp
    IpFileHanler.cpp
    IpFileHanler.h
)

# Include directories
target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link external libraries (e.g., Qt)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Set include directories
target_include_directories(services PRIVATE
    ${CMAKE_SOURCE_DIR}/GUI/src
    ${CMAKE_SOURCE_DIR}/GUI/src/services
    ${CMAKE_SOURCE_DIR}/GUI/src/gui
    ${CMAKE_SOURCE_DIR}/GUI/src/models
    ${CMAKE_SOURCE_DIR}/GUI/src/items
    ${CMAKE_SOURCE_DIR}/GUI/src/state
    ${CMAKE_SOURCE_DIR}/GUI/include/VehicleCompSim/utils
    ${CMAKE_SOURCE_DIR}/Communication/server/include
    ${CMAKE_SOURCE_DIR}/Communication/client/include
    ${CMAKE_SOURCE_DIR}/Communication/utilities
)

target_link_libraries(services PUBLIC Qt6::Core Qt6::Widgets VehicleCompSimUtils server_lib client_lib)
