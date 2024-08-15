cmake_minimum_required(VERSION 3.5)

project(VehicleCompSim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt6 REQUIRED COMPONENTS WebSockets)
find_package(Qt6 REQUIRED COMPONENTS Network)

# Specify the path to the bson library
set(BSON_ROOT "${PROJECT_SOURCE_DIR}/externalLibraries/bson")

# Add the include directory for the bson headers
include_directories(${BSON_ROOT}/include)
# 1
# Include directories for your library headers
include_directories(library_to_user/client)
include_directories(library_to_user/server)
include_directories(library_to_user/utilities)


# Add the library directories for the bson library
#link_directories(${BSON_ROOT}/lib)
link_directories(${BSON_ROOT}/lib/Debug)
link_directories(${BSON_ROOT}/lib/Relese)
#2
# Specify the path to your library
link_directories(library_to_user/lib)

# Define source files
set(COMMON_SOURCES
    src/items/serializableitem.cpp
    src/items/serializableitem.h
    src/client/observer.h
    src/client/observerconfig.h
    src/client/observerconfig.cpp
    
    src/items/baseitem.cpp
    src/items/baseitem.h
    src/items/sensoritem.cpp
    src/items/sensoritem.h
    src/items/connectoritem.cpp
    src/items/connectoritem.h
    src/items/edgeitem.cpp
    src/items/edgeitem.h
    src/gui/popupdialog.cpp
    src/gui/popupdialog.h
    src/gui/popupdialog.ui
    include/VehicleCompSim/utils/runCommand/runCommand.cpp
    include/VehicleCompSim/utils/runCommand/runCommand.h
    include/VehicleCompSim/utils/network.hpp
    
    src/services/runservice.cpp
    src/services/runservice.h
    include/VehicleCompSim/utils/CMakeUtils/getBuildAndRunCommands.cpp
    include/VehicleCompSim/utils/CMakeUtils/getBuildAndRunCommands.h
    src/items/actionsblocker.h
    src/items/actionsblocker.cpp
    include/VehicleCompSim/utils/processControls.cpp
    include/VehicleCompSim/utils/processControls.h
)

# Define client source files
set(CLIENT_SOURCES
    src/client/websocketclient.cpp
    src/client/websocketclient.h
    src/client/main_client.cpp


    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.ui
    src/gui/customwidget.cpp
    src/gui/customwidget.h
    src/gui/customscene.cpp
    src/gui/customscene.h
)

# Define server source files
set(SERVER_SOURCES
    src/server/websocketserver.cpp
    src/server/websocketserver.h
    src/server/main_server.cpp
)

# Create client executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ClientApp
        MANUAL_FINALIZATION
        ${COMMON_SOURCES}
        ${CLIENT_SOURCES}
    )
else()
    add_executable(ClientApp
        ${COMMON_SOURCES}
        ${CLIENT_SOURCES}
    )
endif()

# Create server executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ServerApp
        MANUAL_FINALIZATION
        ${COMMON_SOURCES}
        ${SERVER_SOURCES}
    )
else()
    add_executable(ServerApp
        ${COMMON_SOURCES}
        ${SERVER_SOURCES}
    )
endif()
# 3
# Create the server executable
add_executable(server_exec server_main.cpp)
target_link_libraries(server_exec client_lib server_lib)
# Create the client executable
add_executable(client_exec client_main.cpp)
target_link_libraries(client_exec client_lib server_lib)


# Set include directories
target_include_directories(ClientApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(ServerApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Link libraries
target_link_libraries(ClientApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(ServerApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network)


# Link the BSON library
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ClientApp PRIVATE "${BSON_ROOT}/lib/Debug/bson-1.0.lib")
    target_link_libraries(ServerApp PRIVATE "${BSON_ROOT}/lib/Debug/bson-1.0.lib")
else()
    target_link_libraries(ClientApp PRIVATE "${BSON_ROOT}/lib/Release/bson-1.0.lib")
    target_link_libraries(ServerApp PRIVATE "${BSON_ROOT}/lib/Release/bson-1.0.lib")
endif()


# Set properties for macOS and iOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VehicleCompSim)
endif()

set_target_properties(ClientApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_target_properties(ServerApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Ensure that the executable is run in an external console on Windows
if (WIN32)
    set_target_properties(ClientApp PROPERTIES
        VS_DEBUGGER_COMMAND_ARGUMENTS "--externalConsole"
    )
endif()

# Ensure that the executable is run in an external console on Windows
if (WIN32)
    set_target_properties(ServerApp PROPERTIES
        VS_DEBUGGER_COMMAND_ARGUMENTS "--externalConsole"
    )
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS ClientApp ServerApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ClientApp)
    qt_finalize_executable(ServerApp)
endif()

# # Enable testing
# enable_testing()

# # Find GTest package
# find_package(GTest REQUIRED)

# # Add test executable
# add_executable(VehicleCompSimTests
#     # Add test files here
# )

# # Set include directories for tests
# target_include_directories(VehicleCompSimTests PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# # Link test executable with Google Test and Qt
# target_link_libraries(VehicleCompSimTests PRIVATE
#     GTest::gtest_main
#     Qt${QT_VERSION_MAJOR}::Widgets
# )

# # Add the test
# add_test(NAME VehicleCompSimTests COMMAND VehicleCompSimTests)

