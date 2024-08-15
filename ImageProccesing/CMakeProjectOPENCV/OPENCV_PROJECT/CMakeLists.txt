cmake_minimum_required(VERSION 3.10)
project(OPENCV_PROJECT)

# הגדרת נתיב OpenCV
set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

# חיפוש אחר OpenCV
find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
  message(STATUS "OpenCV version: ${OpenCV_VERSION}")
else()
  message(FATAL_ERROR "OpenCV not found")
endif()


# הגדרת נתיב לדאטה נוסף
set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

# הגדרת קבצי מקור וכתובת
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# הוספת קבצי מקור
set(SOURCES
    src/main.cpp
    src/yolov3.cpp
)

# יצירת הבינארי
add_executable(CMakeProjectOPENCV ${SOURCES})


# הגדרת תקן C++
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProjectOPENCV PROPERTY CXX_STANDARD 20)
endif()

# חיבור ספריות OpenCV לפרויקט
target_link_libraries(CMakeProjectOPENCV ${OpenCV_LIBS})

# הגדרת משתנה סביבה לקבצים
target_compile_definitions(CMakeProjectOPENCV PRIVATE DATA_DIR="${DATA_DIR}")
