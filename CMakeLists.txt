
cmake_minimum_required(VERSION 3.4)
project(hsm)

set(CMAKE_CXX_STANDARD 17)
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


enable_testing()
add_subdirectory(aes)
add_subdirectory(BigNum)
add_subdirectory(SHA-256)
add_subdirectory(rsa)
add_subdirectory(test)
add_subdirectory(benchmark)


# add_executable(main main.cpp)

# Link the BigNum and RSA libraries with the main executable
# target_link_libraries(main PRIVATE BigNum RSA)