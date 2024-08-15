cmake_minimum_required(VERSION 3.10)

project(ConditionsManagerProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
include_directories(resource) 

add_executable(ConditionsManagerProject src/main.cpp)

target_link_libraries(ConditionsManagerProject PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

add_dependencies(ConditionsManagerProject MainComputer)
