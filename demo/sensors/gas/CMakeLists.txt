cmake_minimum_required(VERSION 3.16)
project(gas)


add_subdirectory(../../Communication Communication_build)

add_executable(gas
    main.cpp
    ../vehicle_components_id_config.h
)



target_include_directories(gas
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Communication/client/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Communication/utilities
)

target_link_libraries(gas PRIVATE
    client_lib
)
