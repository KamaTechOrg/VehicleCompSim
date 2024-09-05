add_library(RSA src/rsaBigNum.cpp src/RSA.cpp)

target_include_directories(RSA PUBLIC include)

add_executable(RSATest test/RSA_test.cpp)
add_executable(RSA_BigNumTest test/RSA_BigNum_test.cpp)

target_link_libraries(RSATest RSA)

add_test(NAME RSATest COMMAND RSATest)
