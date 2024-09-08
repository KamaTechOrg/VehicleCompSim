find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

# Define the static library target
add_library(VehicleCompSimUtils STATIC
    runCommand/runCommand.cpp
    runCommand/runCommand.h
    network.hpp
    CMakeUtils/getBuildAndRunCommands.cpp
    CMakeUtils/getBuildAndRunCommands.h
    processControls.cpp
    processControls.h
)

# Set include directories for the library
target_include_directories(VehicleCompSimUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS}
)

target_link_libraries(VehicleCompSimUtils PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# Link the BSON library to the library target
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #target_link_libraries(VehicleCompSimUtils PRIVATE "${BSON_ROOT}/lib/Debug/bson-1.0.lib")
else()
    #target_link_libraries(VehicleCompSimUtils PRIVATE "${BSON_ROOT}/lib/Release/bson-1.0.lib")
endif()
