add_library(BigNum src/BigNumWithMinus.cpp src/BigNum.cpp)

target_include_directories(BigNum PUBLIC include)

add_executable(BigNumTest test/BigNum_test.cpp)
add_executable(BigNumWithMinusTest test/BigNumWithMinus_test.cpp)

target_link_libraries(BigNumTest BigNum)

add_test(NAME BigNumTest COMMAND BigNumTest)
