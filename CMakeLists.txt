cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Create a library from the utilities
add_library(utilities STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities/socket.cpp
)

# Specify include directories for the utilities library
target_include_directories(utilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
)

# Helper function to create executables from sources containing main()
function(create_executable_from_sources source_dir executable_prefix)
    file(GLOB SOURCES ${source_dir}/*.cpp)
    foreach(source ${SOURCES})
        # Check if the source file contains a main function (simple approach)
        file(READ ${source} file_content)
        if(file_content MATCHES "int\\s+main\\s*\\(")
            get_filename_component(executable_name ${source} NAME_WE)
            set(executable_name "${executable_prefix}_${executable_name}")
            
            add_executable(${executable_name} ${source})
            target_link_libraries(${executable_name} PRIVATE utilities)
            target_include_directories(${executable_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/client/include)
        endif()
    endforeach()
endfunction()

# Create client executables
create_executable_from_sources(${CMAKE_CURRENT_SOURCE_DIR}/client/src client)

# Create server executables
create_executable_from_sources(${CMAKE_CURRENT_SOURCE_DIR}/server/src server)

# Create utility executables if needed
create_executable_from_sources(${CMAKE_CURRENT_SOURCE_DIR}/utilities utility)
