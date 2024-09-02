cmake_minimum_required(VERSION 3.16)
project(MainComputer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Define source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/GUI/src/*.cpp"
)

# Define header files
file(GLOB_RECURSE HEADERS 
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/GUI/include/*.h"
)

qt_add_executable(MainComputer
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(MainComputer PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/GUI/include
    ${CMAKE_SOURCE_DIR}/resources
)

target_link_libraries(MainComputer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(MainComputer PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE FALSE
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(MainComputer PROPERTIES
        WIN32_EXECUTABLE FALSE
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS MainComputer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)