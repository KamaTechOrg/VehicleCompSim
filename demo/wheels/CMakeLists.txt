cmake_minimum_required(VERSION 3.16)
project(wheels)


add_subdirectory(../../Communication Communication_build)

add_executable(wheels
    main.cpp
    ../vehicle_components_id_config.h
)



target_include_directories(wheels
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Communication/client/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Communication/utilities
)

target_link_libraries(wheels PRIVATE
    client_lib
)
