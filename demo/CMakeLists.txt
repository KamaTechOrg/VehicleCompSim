cmake_minimum_required(VERSION 3.16)
project(demo)


add_subdirectory(../Communication Communication_build)

add_executable(demo
    main.cpp
)



target_include_directories(demo
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Communication/client/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../Communication/utilities
)

target_link_libraries(demo PRIVATE
    client_lib
)
