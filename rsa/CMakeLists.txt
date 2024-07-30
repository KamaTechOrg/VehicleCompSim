add_library(RSA OBJECT RSA.cpp rsaBigNum.cpp)
target_link_libraries(RSA PUBLIC BigNum)
target_include_directories(RSA 
    INTERFACE .
)
