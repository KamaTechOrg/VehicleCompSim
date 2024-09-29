
cmake_minimum_required(VERSION 3.4)
project(HSM)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.15.0/googletest-1.15.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include (CMakeForceCompiler)

MACRO(SET_COMPILER_TO_ICPX)
    if(UNIX)
            # Direct CMake to use icpx rather than the default C++ compiler/linker
        set(CMAKE_CXX_COMPILER icpx)
    else() # Windows
        # Force CMake to use icx-cl rather than the default C++ compiler/linker 
        # (needed on Windows only)
        include (CMakeForceCompiler)
        CMAKE_FORCE_CXX_COMPILER (icx-cl IntelDPCPP)
        include (Platform/Windows-Clang)
    endif()
ENDMACRO()

add_library(sycl_flags INTERFACE)
target_compile_definitions(sycl_flags INTERFACE SYCL_ENABLED=1)
target_link_options(sycl_flags INTERFACE "-fsycl")
target_compile_options(sycl_flags INTERFACE "-fsycl")


#testing
# add_executable(APITest test/API_test.cpp)
# target_link_libraries(APITest API GTest::gtest_main)
# target_link_libraries(API RSA)
# target_link_libraries(API AES)
# target_link_libraries(API)



# gtest_discover_tests(APITest)



enable_testing()
add_subdirectory(AES)
add_subdirectory(BigNum)
add_subdirectory(SHA256)
add_subdirectory(RSA)
add_subdirectory(SHA_API)
# add_subdirectory(Benchmark)
add_subdirectory(HSM_Enums)

# API
add_library(HSM src/HSM.cpp src/ID.cpp)
target_include_directories(HSM PUBLIC include)
target_link_libraries(HSM AES_API RSA SHA_API HSM_Enums)


#testing
add_executable(API_TEST test/API_test.cpp)
target_link_libraries(API_TEST HSM AES_API GTest::gtest_main)
gtest_discover_tests(API_TEST)



# add_executable(main main.cpp)

# Link the BigNum and RSA libraries with the main executable
# target_link_libraries(main PRIVATE BigNum RSA)