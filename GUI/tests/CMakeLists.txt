include(FetchContent)

# Declare and fetch googletest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

# # Disable pthreads for googletest
# set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

# Disable building tests for googletest
set(gtest_build_tests OFF CACHE BOOL "" FORCE)
set(gmock_build_tests OFF CACHE BOOL "" FORCE)

# Add the GTest and GMock libraries
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_library(GMock::GMock INTERFACE IMPORTED)
target_link_libraries(GMock::GMock INTERFACE gmock_main)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Add the test executable
add_executable(services_test
  runservice_test.cpp
  test_parser.cpp
  test_saveAndLoad.cpp
  test_DB_handler.cpp
)

# Link the test executable with GTest, GMock, and your library
target_link_libraries(services_test
 PRIVATE
  GTest::GTest
  GMock::GMock
  VehicleCompSimCore
  Qt6::Core
  Qt6::Widgets
  VehicleCompSimUtils
  server_lib
  client_lib
  bson_static
)

# Set include directories
target_include_directories(services_test PRIVATE
${CMAKE_SOURCE_DIR}/GUI/src
${CMAKE_SOURCE_DIR}/GUI/src/services
${CMAKE_SOURCE_DIR}/GUI/src/gui
${CMAKE_SOURCE_DIR}/GUI/src/models
${CMAKE_SOURCE_DIR}/GUI/src/items
${CMAKE_SOURCE_DIR}/GUI/src/state
${CMAKE_SOURCE_DIR}/GUI/include/VehicleCompSim/utils
${CMAKE_SOURCE_DIR}/Communication/server/include
${CMAKE_SOURCE_DIR}/Communication/client/include
${CMAKE_SOURCE_DIR}/Communication/utilities
)

# Add the test
add_test(services_test services_test)