add_library(RSA RSA.cpp)
add_library(rsaBigNum rsaBigNum.cpp)
target_include_directories(rsaBigNum PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(RSA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
