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

# # Disable building tests for gmock
# set(gmock_build_tests OFF CACHE BOOL "" FORCE)

# Add the GTest and GMock libraries
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_library(GMock::GMock INTERFACE IMPORTED)
target_link_libraries(GMock::GMock INTERFACE gmock_main)

# Add the test executable
add_executable(runservice_test runservice_test.cpp)

# Link the test executable with GTest, GMock, and your library
target_link_libraries(runservice_test
 PRIVATE
  GTest::GTest
  GMock::GMock
  services
)

# Set include directories
target_include_directories(runservice_test PRIVATE
    ${CMAKE_SOURCE_DIR}/GUI/src
    ${CMAKE_SOURCE_DIR}/GUI/src/services
    ${CMAKE_SOURCE_DIR}/GUI/include/VehicleCompSim/utils
)

# Add the test
add_test(runservice_gtests runservice_test)