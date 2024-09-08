# Define the project name and version
project(TESTS VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the test files
file(GLOB TEST_FILES "*.cpp")

# Add the test executable
add_executable(tests ${TEST_FILES})

# Set include directories
target_include_directories(tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

target_sources(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/SimpleCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/AndCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/CompositeCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/OrCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/ContainsCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/GreaterThanCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/SmallerThanCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/StartsWithCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/EndsWithCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/EqualsToCondition.cpp
    ${CMAKE_SOURCE_DIR}/src/ConditionsFactory.cpp)
# Set compile definitions for Doctest
target_compile_definitions(tests PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

# Enable testing
enable_testing()

# Add test
add_test(NAME tests COMMAND tests)

# Set CTest arguments
set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")

target_link_libraries(tests PRIVATE
    Qt6::Core
)