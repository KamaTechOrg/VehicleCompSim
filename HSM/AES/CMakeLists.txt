add_library(AES STATIC 
    src/AES_block_encrypt.cpp 
    src/AES_chaining_stream.cpp )

add_library(AES_API src/AES_API.cpp)
target_include_directories(AES PUBLIC include)

target_link_libraries(AES_API HSM)
target_link_libraries(AES_API AES)
target_link_libraries(AES_API HSM_Enums)
target_include_directories(AES_API PUBLIC include)


if(DEFINED SYCL_ENABLED AND (NOT(SYCL_ENABLED EQUAL 0)))
    SET_COMPILER_TO_ICPX()
    target_link_libraries(AES PRIVATE sycl_flags)
endif()

add_executable(aes_test test/AES_test.cpp)
target_link_libraries(aes_test AES GTest::gtest_main)
gtest_discover_tests(aes_test)


# find_package(TBB REQUIRED) # std::execution::par
# target_link_libraries(AES PRIVATE TBB::tbb)
