add_library(aes INTERFACE)
target_include_directories(aes 
    INTERFACE .
)

target_precompile_headers(aes 
    INTERFACE aes.hpp
)