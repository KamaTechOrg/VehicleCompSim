add_library(AES STATIC src/aes.cpp src/aes_text_encrypt.cpp src/AES_API.cpp)

target_include_directories(AES PUBLIC include)

add_executable(aes_test test/aes_test.cpp)
target_link_libraries(aes_test AES GTest::gtest_main)
gtest_discover_tests(aes_test)

if(DEFINED SYCL_ENABLED AND (NOT(SYCL_ENABLED EQUAL 0)))
    SET_COMPILER_TO_ICPX()
    target_link_libraries(AES PRIVATE sycl_flags)
    target_link_libraries(aes_test sycl_flags)
endif()


find_package(TBB REQUIRED) # std::execution::par
target_link_libraries(AES PRIVATE TBB::tbb)
