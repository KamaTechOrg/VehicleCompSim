set(BENCHMARK_ENABLE_TESTING 0)
include(FetchContent)
FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main) # need master for benchmark::benchmark

FetchContent_MakeAvailable(
        googlebenchmark)

add_executable(bigNum_bench bigNum_bench.cpp)
target_link_libraries(bigNum_bench BigNum benchmark::benchmark)

add_executable(aes_bench aes_bench.cpp)
target_link_libraries(aes_bench aes benchmark::benchmark)
