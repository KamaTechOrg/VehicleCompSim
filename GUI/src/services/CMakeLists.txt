cmake_minimum_required(VERSION 3.10)

# Include the runservice directory
add_subdirectory(runservice)

# Define the source files for services
set(SERVICES_SOURCES
    qtlogger.cpp
    qtlogger.h
    SimulationRecorder.cpp
    SimulationRecorder.h
    SimulationReplayer.cpp
    SimulationReplayer.h
    DB_handler.cpp
    DB_handler.h
    saveAndLoad.cpp
    saveAndLoad.h
    parser.cpp
    parser.h
    ${RUNSERVICE_SOURCES}
)

# Define the library
add_library(services STATIC ${SERVICES_SOURCES})

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

target_link_libraries(services 
PUBLIC 
    Qt6::Core 
    Qt6::Widgets 
    VehicleCompSimUtils
PRIVATE
    server_lib 
    client_lib
)