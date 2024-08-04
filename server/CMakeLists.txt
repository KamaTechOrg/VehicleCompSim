# client/CMakeLists.txt
add_executable(server
    src/main.cpp
)

target_include_directories(server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server PRIVATE utils)