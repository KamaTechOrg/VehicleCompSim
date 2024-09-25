cmake_minimum_required(VERSION 3.10)
project(OPENCV_PROJECT)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)


#set(OpenCV_DIR "C:/Users/dmaro/Downloads/opencv_library_backup/install/x64/vc17/lib")
set(OpenCV_DIR "C:/opencv4.10.0/opencv_build/install/x64/vc17/lib")

find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
  message(STATUS "OpenCV version: ${OpenCV_VERSION}")
else()
  message(FATAL_ERROR "OpenCV not found")
endif()

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set source files excluding main.cpp
set(SOURCES
    src/ApproachDangerZone.cpp
    src/Distance.cpp
    src/Manager.cpp
    src/NewPrediction.cpp
    src/Poligon.cpp
    src/Prediction.cpp
    src/stereo_main.cpp
    src/tracking.cpp
    src/Visualization.cpp
    src/yolov3.cpp
    src/yolov5n.cpp
    src/lane_departure.cpp
    src/laneDetection.cpp
    src/runLaneDetection.cpp

)

# Add the static library
add_library(CMakeProjectOPENCV STATIC ${SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProjectOPENCV PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(CMakeProjectOPENCV ${OpenCV_LIBS})

target_compile_definitions(CMakeProjectOPENCV PRIVATE DATA_DIR="${DATA_DIR}")

# Optionally, add executable linking to the static library
add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp CMakeProjectOPENCV ${OpenCV_LIBS})
