# Define the project name and version
project(AndConditionTest VERSION 1.0)

# Find the test files
file(GLOB TEST_FILES "*.cpp")

# Add the test executable
add_executable(test_validation_and_condition ${TEST_FILES})

# Set include directories
target_include_directories(test_validation_and_condition PRIVATE 
    ${CMAKE_SOURCE_DIR}/MainComputer/include
    ${CMAKE_SOURCE_DIR}/MainComputer/resources
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

target_sources(test_validation_and_condition PRIVATE
    ${CMAKE_SOURCE_DIR}/MainComputer/src/SimpleCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/AndCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/CompositeCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/OrCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/ContainsCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/GreaterThanCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/SmallerThanCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/StartsWithCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/EndsWithCondition.cpp
    ${CMAKE_SOURCE_DIR}/MainComputer/src/EqualsToCondition.cpp)
# Set compile definitions for Doctest
target_compile_definitions(test_validation_and_condition PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

# Enable testing
enable_testing()

# Add test
add_test(NAME test_validation_and_condition COMMAND test_validation_and_condition)

# Set CTest arguments
set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")

