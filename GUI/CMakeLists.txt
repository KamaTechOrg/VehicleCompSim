cmake_minimum_required(VERSION 3.5)

project(VehicleCompSim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)


find_package(Qt6 REQUIRED COMPONENTS Widgets WebSockets Sql)

qt_standard_project_setup()

# Include the FetchDependencies.cmake file
include(${CMAKE_CURRENT_SOURCE_DIR}/FetchDependencies.cmake)

include_directories(${mongo-c-driver_SOURCE_DIR}/src/libbson/src)
include_directories(${mongo-c-driver_BINARY_DIR}/src/libbson/src)

enable_testing()

# Add subdirectories
add_subdirectory(tests)
add_subdirectory(include/VehicleCompSim/utils)

add_subdirectory(src)

# Create client executable
qt_add_executable(ClientApp
    MANUAL_FINALIZATION
    src/main.cpp
)

# Set include directories
target_include_directories(ClientApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VehicleCompSim/utils
)

# Link libraries
target_link_libraries(ClientApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::WebSockets
    VehicleCompSimUtils
    VehicleCompSimCore
    services
    bson_static
)

# Set properties for Windows
if(WIN32)
    set_target_properties(ClientApp PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        # VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS ClientApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# # Deploy Qt dependencies
# set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
# include(InstallRequiredSystemLibraries)
# set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR})


# Generate deploy script
qt_generate_deploy_app_script(
    TARGET ClientApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# Finalize the executable
qt_finalize_executable(ClientApp)
