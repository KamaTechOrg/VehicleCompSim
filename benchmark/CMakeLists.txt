include(FetchContent)
set(BENCHMARK_ENABLE_TESTING 0)
FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main) # need master for benchmark::benchmark
FetchContent_MakeAvailable(
        googlebenchmark)

add_executable(bigNum_bench bigNum_bench.cpp)
target_link_libraries(bigNum_bench BigNum benchmark::benchmark)

add_executable(aes_bench aes_bench.cpp)
target_link_libraries(aes_bench PRIVATE aes benchmark::benchmark)

if(DEFINED SYCL_ENABELED AND (NOT(SYCL_ENABELED EQUAL 0)))
        SET_COMPILER_TO_ICPX()
        target_link_libraries(aes_bench PRIVATE sycl_flags)
endif()
