cmake_minimum_required(VERSION 3.10)

# Set the project name for the root
project(VehicleCompSim)


if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64")
    add_definitions(-D_AMD64_)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86")
    add_definitions(-D_X86_)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "ARM")
    add_definitions(-D_ARM_)
else()
    message(FATAL_ERROR "No Target Architecture defined")
endif()

enable_testing()
set(CTEST_OUTPUT_ON_FAILURE TRUE)

# Add subdirectories
add_subdirectory(Communication)
add_subdirectory(MainComputer)
add_subdirectory(GUI)
add_subdirectory(GUI/tests)
