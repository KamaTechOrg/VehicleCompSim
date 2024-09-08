cmake_minimum_required(VERSION 3.5)

project(VehicleCompSim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebSockets)

qt_standard_project_setup()

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# find_package(Qt6 REQUIRED COMPONENTS WebSockets)
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Specify the path to the bson library
set(BSON_ROOT "${PROJECT_SOURCE_DIR}/externalLibraries/bson")

# Add the include directory for the bson headers
include_directories(${BSON_ROOT}/include)

# Add the library directories for the bson library
link_directories(${BSON_ROOT}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>)

enable_testing()

# Add subdirectories
add_subdirectory(tests)
add_subdirectory(include/VehicleCompSim/utils)
add_subdirectory(src)

# Create client executable
qt_add_executable(ClientApp
    MANUAL_FINALIZATION
    src/client/main_client.cpp
)

# Set include directories
target_include_directories(ClientApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VehicleCompSim/utils
)

# set(BSON_DLL_DEBUG "${BSON_ROOT}/lib/Debug/libbson-1.0.dll")
# set(BSON_DLL_RELEASE "${BSON_ROOT}/lib/Release/libbson-1.0.dll")

# Link libraries
target_link_libraries(ClientApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebSockets
    VehicleCompSimUtils
    VehicleCompSimCore
    services
    $<IF:$<CONFIG:Debug>,${BSON_ROOT}/lib/Debug/bson-1.0.lib,${BSON_ROOT}/lib/Release/bson-1.0.lib>
)

# Set properties for Windows
if(WIN32)
    set_target_properties(ClientApp PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        # VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()

# # Copy BSON DLL to output directory
# add_custom_command(TARGET ClientApp POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E echo "Attempting to copy BSON DLL..."
#     COMMAND ${CMAKE_COMMAND} -E copy $<IF:$<CONFIG:Debug>,${BSON_DLL_DEBUG},${BSON_DLL_RELEASE}> $<TARGET_FILE_DIR:ClientApp>
#     COMMAND ${CMAKE_COMMAND} -E echo "BSON DLL copy command completed."
# )

# Installation
include(GNUInstallDirs)
install(TARGETS ClientApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Copy BSON DLL during installation
install(FILES $<IF:$<CONFIG:Debug>,${BSON_ROOT}/lib/Debug/libbson-1.0.dll,${BSON_ROOT}/lib/Release/libbson-1.0.dll>
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Generate deploy script
qt_generate_deploy_app_script(
    TARGET ClientApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# Finalize the executable
qt_finalize_executable(ClientApp)
