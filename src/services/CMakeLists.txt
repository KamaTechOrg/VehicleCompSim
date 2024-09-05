cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(services STATIC
    runservice.cpp
)

# Include directories
target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link external libraries (e.g., Qt)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)
target_link_libraries(services PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets VehicleCompSimUtils)

# Set include directories
target_include_directories(services PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/services
    ${CMAKE_SOURCE_DIR}/include/VehicleCompSim/utils
)