cmake_minimum_required(VERSION 3.16)
project(breaks)


add_subdirectory(../../Communication Communication_build)

add_executable(breaks
    main.cpp
    ../vehicle_components_id_config.h
)



target_include_directories(breaks
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Communication/client/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Communication/utilities
)

target_link_libraries(breaks PRIVATE
    client_lib
)
