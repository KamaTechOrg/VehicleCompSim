# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CMakeProjectOPENCV")

# Include sub-projects.
add_subdirectory ("OPENCV_PROJECT")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/OPENCV_PROJECT/include)


link_directories(../../Communication/User_Directory/lib/win)


# Include directories for your library headers
include_directories(../../Communication/User_Directory/client)
include_directories(../../Communication/User_Directory/utilities)

add_executable(MyApp main.cpp)

#
set_target_properties(MyApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)

set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/opencv_library_backup/install/x64/vc17/lib")

file(COPY "${OpenCV_DIR}/../bin" DESTINATION "${CMAKE_BINARY_DIR}")

#


# Create the client executable
target_link_libraries(MyApp client_lib)
target_link_libraries(MyApp CMakeProjectOPENCV ${OpenCV_LIBS})

# Set the DATA_DIR for MyApp
target_compile_definitions(MyApp PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")




