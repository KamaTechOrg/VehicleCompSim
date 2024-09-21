cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(services STATIC
    runservice.cpp
)

# Include directories
target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link external libraries (e.g., Qt)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
target_link_libraries(services PUBLIC Qt6::Core Qt6::Widgets VehicleCompSimUtils)

# Set include directories
target_include_directories(services PRIVATE
    ${CMAKE_SOURCE_DIR}/GUI/src
    ${CMAKE_SOURCE_DIR}/GUI/src/services
    ${CMAKE_SOURCE_DIR}/GUI/src/gui
    ${CMAKE_SOURCE_DIR}/GUI/src/models
    ${CMAKE_SOURCE_DIR}/GUI/src/items
    ${CMAKE_SOURCE_DIR}/GUI/src/state
    ${CMAKE_SOURCE_DIR}/GUI/include/VehicleCompSim/utils
)