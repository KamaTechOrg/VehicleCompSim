add_library(AES STATIC src/aes.cpp src/aes_text_encrypt.cpp)

target_include_directories(AES PUBLIC include)

if(DEFINED SYCL_ENABLED AND (NOT(SYCL_ENABLED EQUAL 0)))
    SET_COMPILER_TO_ICPX()
    target_link_libraries(AES PRIVATE sycl_flags)
endif()

# find_package(TBB REQUIRED) # std::execution::par
# target_link_libraries(aes PRIVATE TBB::tbb)
