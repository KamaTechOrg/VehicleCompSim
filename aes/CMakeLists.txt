add_library(aes STATIC aes.cpp aes_text_encrypt.cpp)

target_include_directories(aes 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

if(DEFINED SYCL_ENABELED AND (NOT(SYCL_ENABELED EQUAL 0)))
    SET_COMPILER_TO_ICPX()
    target_link_libraries(aes PRIVATE sycl_flags)
endif()

# find_package(TBB REQUIRED) # std::execution::par
# target_link_libraries(aes PRIVATE TBB::tbb)
