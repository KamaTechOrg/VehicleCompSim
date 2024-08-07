add_library(BigNum BigNum.cpp)
add_library(BigNumWithMinus BigNumWithMinus.hpp)
target_include_directories(BigNumWithMinus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(BigNum PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
