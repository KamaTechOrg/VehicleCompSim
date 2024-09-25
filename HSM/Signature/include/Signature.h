#pragma once
#include <vector<u_char>>
#include <vector>


class Signature {
public:
    static std::vector<u_char> sha256_sign(
        const std::vector<u_char>& message,
        const std::vector<u_char>& sigAlg,
        const std::vector<u_char>& key
    );

    static bool sha256_verify(
        const std::vector<u_char>& message,
        const std::vector<u_char>& signature,
        const std::vector<u_char>& sigAlg,
        const std::vector<u_char>& key
    );

    static std::vector<u_char> sha3_256_sign(
        const std::vector<u_char>& message,
        const std::vector<u_char>& sigAlg,
        const std::vector<u_char>& key
    );
    
    static bool sha3_256_verify(
        const std::vector<u_char>& message,
        const std::vector<u_char>& signature,
        const std::vector<u_char>& sigAlg,
        const std::vector<u_char>& key
    );
    
};