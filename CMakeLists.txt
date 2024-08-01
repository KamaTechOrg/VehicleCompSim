cmake_minimum_required(VERSION 3.5)

project(VehicleCompSim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


# Specify the path to the bson library
set(BSON_ROOT "${PROJECT_SOURCE_DIR}/externalLibraries/bson")

# Add the include directory for the bson headers
include_directories(${BSON_ROOT}/include)

# Add the library directories for the bson library
#link_directories(${BSON_ROOT}/lib)
link_directories(${BSON_ROOT}/lib/Debug)
link_directories(${BSON_ROOT}/lib/Relese)


# Define source files
set(PROJECT_SOURCES
    src/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.ui
    src/gui/popupdialog.cpp
    src/gui/popupdialog.h
    src/gui/popupdialog.ui
    src/gui/customwidget.cpp
    src/gui/customwidget.h
    src/gui/customscene.cpp
    src/gui/customscene.h
    src/items/baseitem.cpp
    src/items/baseitem.h
    src/items/sensoritem.cpp
    src/items/sensoritem.h
    src/items/connectoritem.cpp
    src/items/connectoritem.h
    src/items/edgeitem.cpp
    src/items/edgeitem.h
)

# Add executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VehicleCompSim
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/VehicleCompSim/utils/network.hpp
        include/VehicleCompSim/utils/runCommand/runCommand.cpp include/VehicleCompSim/utils/runCommand/runCommand.h include/VehicleCompSim/utils/network.hpp   src/gui/customscene.cpp src/gui/customscene.h src/gui/customwidget.cpp src/gui/customwidget.h src/gui/mainwindow.cpp src/gui/mainwindow.h src/gui/mainwindow.ui src/gui/popupdialog.cpp src/gui/popupdialog.h src/gui/popupdialog.ui src/items/baseitem.cpp src/items/baseitem.h src/items/connectoritem.cpp src/items/connectoritem.h src/items/edgeitem.cpp src/items/edgeitem.h src/items/sensoritem.cpp src/items/sensoritem.h src/main.cpp
        src/services/runservice.cpp src/services/runservice.h
    )
else()
    if(ANDROID)
        add_library(VehicleCompSim SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(VehicleCompSim
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Set include directories
target_include_directories(VehicleCompSim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(VehicleCompSim PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# Link the BSON library
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(VehicleCompSim PRIVATE "${BSON_ROOT}/lib/Debug/bson-1.0.lib")
else()
    target_link_libraries(VehicleCompSim PRIVATE "${BSON_ROOT}/lib/Release/bson-1.0.lib")
endif()


# Set properties for macOS and iOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VehicleCompSim)
endif()
set_target_properties(VehicleCompSim PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Installation
include(GNUInstallDirs)
install(TARGETS VehicleCompSim
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VehicleCompSim)
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
