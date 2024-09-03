cmake_minimum_required(VERSION 3.5)

project(VehicleCompSim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

# # Use Qt6 by default
# set(QT_DEFAULT_MAJOR_VERSION 6)

# find_package(Qt6 REQUIRED COMPONENTS Widgets WebSockets Network)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt6 REQUIRED COMPONENTS WebSockets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

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
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS ClientApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize the executable
qt_finalize_executable(ClientApp)

# Use windeployqt to deploy Qt dependencies on Windows
if(WIN32)
    get_target_property(_qmake_executable Qt::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    add_custom_command(TARGET ClientApp POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            --verbose 0
            --no-compiler-runtime
            --no-opengl-sw
            \"$<TARGET_FILE:ClientApp>\"
        COMMENT "Deploying Qt libraries using windeployqt for target ${ClientApp}"
    )
endif()
# cmake_minimum_required(VERSION 3.5)

# project(VehicleCompSim VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_BUILD_TYPE Debug)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# find_package(Qt6 REQUIRED COMPONENTS WebSockets)
# find_package(Qt6 REQUIRED COMPONENTS Network)
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# # Specify the path to the bson library
# set(BSON_ROOT "${PROJECT_SOURCE_DIR}/externalLibraries/bson")

# # Add the include directory for the bson headers
# include_directories(${BSON_ROOT}/include)

# # Add the library directories for the bson library
# link_directories(${BSON_ROOT}/lib/Debug)
# link_directories(${BSON_ROOT}/lib/Release)

# enable_testing()

# # Add the test subdirectories
# add_subdirectory(tests)
# add_subdirectory(include/VehicleCompSim/utils)
# add_subdirectory(src/services)

# # Define source files
# set(COMMON_SOURCES
#     src/items/serializableitem.cpp
#     src/items/serializableitem.h
#     src/client/observer.h
#     src/client/observerconfig.h
#     src/client/observerconfig.cpp
    
#     src/items/baseitem.cpp
#     src/items/baseitem.h
#     src/items/sensoritem.cpp
#     src/items/sensoritem.h
#     src/items/connectoritem.cpp
#     src/items/connectoritem.h
#     src/items/edgeitem.cpp
#     src/items/edgeitem.h
#     src/gui/popupdialog.cpp
#     src/gui/popupdialog.h
#     src/gui/popupdialog.ui
#     include/VehicleCompSim/utils/runCommand/runCommand.cpp
#     include/VehicleCompSim/utils/runCommand/runCommand.h
#     include/VehicleCompSim/utils/network.hpp

#     src/services/runservice.cpp
#     src/services/runservice.h
#     include/VehicleCompSim/utils/CMakeUtils/getBuildAndRunCommands.cpp
#     include/VehicleCompSim/utils/CMakeUtils/getBuildAndRunCommands.h
#     src/items/actionsblocker.h
#     src/items/actionsblocker.cpp
#     include/VehicleCompSim/utils/processControls.cpp
#     include/VehicleCompSim/utils/processControls.h
    
#     src/items/actionsblocker.h
#     src/items/actionsblocker.cpp
# )

# # Define client source files
# set(CLIENT_SOURCES
#     src/client/websocketclient.cpp
#     src/client/websocketclient.h
#     src/client/main_client.cpp
#     src/client/IActionHandler.h
#     src/client/IdentifyHandler.h
#     src/client/IdentifyHandler.cpp
#     src/client/AddItemHandler.h
#     src/client/AddItemHandler.cpp
#     src/client/DeleteItemHandler.h
#     src/client/DeleteItemHandler.cpp
#     src/client/ModifyItemHandler.h
#     src/client/ModifyItemHandler.cpp
#     src/client/RunHandler.h
#     src/client/RunHandler.cpp

#     src/gui/mainwindow.cpp
#     src/gui/mainwindow.h
#     src/gui/mainwindow.ui
#     src/gui/customwidget.cpp
#     src/gui/customwidget.h
#     src/gui/customscene.cpp
#     src/gui/customscene.h
#     src/gui/LiveUpdate.cpp
#     src/gui/LogReader.cpp
#     src/gui/SimulationRecorder.cpp
#     src/gui/SimulationReplayer.cpp
#     src/gui/SimulationControlPanel.h
#     src/gui/SimulationControlPanel.cpp
# )

# # Create client executable
# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(ClientApp
#         MANUAL_FINALIZATION
#         ${COMMON_SOURCES}
#         ${CLIENT_SOURCES}
#     )
# else()
#     add_executable(ClientApp
#         ${COMMON_SOURCES}
#         ${CLIENT_SOURCES}
#     )
# endif()

# # Set include directories
# target_include_directories(ClientApp PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/VehicleCompSim/utils
# )

# # Link libraries
# target_link_libraries(ClientApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets VehicleCompSimUtils services)

# # Link the BSON library
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     target_link_libraries(ClientApp PRIVATE "${BSON_ROOT}/lib/Debug/bson-1.0.lib")
# else()
#     target_link_libraries(ClientApp PRIVATE "${BSON_ROOT}/lib/Release/bson-1.0.lib")
# endif()

# # Set properties for macOS and iOS
# if(${QT_VERSION} VERSION_LESS 6.1.0)
#   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VehicleCompSim)
# endif()

# set_target_properties(ClientApp PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# # Ensure that the executable is run in an external console on Windows
# if (WIN32)
#     set_target_properties(ClientApp PROPERTIES
#         VS_DEBUGGER_COMMAND_ARGUMENTS "--externalConsole"
#     )
# endif()

# # Installation
# include(GNUInstallDirs)
# install(TARGETS ClientApp
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(ClientApp)
# endif()

# # # Use windeployqt to deploy Qt dependencies
# # if(WIN32)
# #   add_custom_command(TARGET ClientApp POST_BUILD
# #       COMMAND ${Qt6_DIR}/../../../bin/windeployqt.exe --release $<TARGET_FILE:ClientApp>
# #   )
  
# #   add_custom_command(TARGET ServerApp POST_BUILD
# #       COMMAND ${Qt6_DIR}/../../../bin/windeployqt.exe --release $<TARGET_FILE:ServerApp>
# #   )
# # endif()
