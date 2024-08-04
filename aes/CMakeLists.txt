add_library(aes STATIC aes.cpp)
target_include_directories(aes 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(TBB REQUIRED) # std::execution::par
target_link_libraries(aes INTERFACE TBB::tbb)
