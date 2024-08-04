include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.15.0/googletest-1.15.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)


add_executable(BigNum_test BigNum_test.cpp)
target_link_libraries(BigNum_test BigNum)
list(APPEND test_targets BigNum_test)

add_executable(RSA_test RSA_test.cpp)
target_link_libraries(RSA_test RSA BigNum)
list(APPEND test_targets RSA_test)

add_executable(aes_test aes_test.cpp)
target_link_libraries(aes_test aes)
list(APPEND test_targets aes_test)


if(DEFINED SYCL_ENABELED AND (NOT(SYCL_ENABELED EQUAL 0)))
    SET_COMPILER_TO_ICPX()
    target_link_libraries(aes_test sycl_flags)
endif()


foreach(test_target IN LISTS test_targets)
  target_link_libraries(${test_target} GTest::gtest_main)
  gtest_discover_tests(${test_target})
endforeach()
set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
