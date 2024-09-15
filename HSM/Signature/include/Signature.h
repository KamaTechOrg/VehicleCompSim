#pragma once
#include <string>
#include <vector>


class Signature {
public:
    static std::string sha256_sign(
        const std::string& message,
        const std::string& sigAlg,
        const std::string& key
    );

    static bool sha256_verify(
        const std::string& message,
        const std::string& signature,
        const std::string& sigAlg,
        const std::string& key
    );

    static std::string sha3_256_sign(
        const std::string& message,
        const std::string& sigAlg,
        const std::string& key
    );
    
    static bool sha3_256_verify(
        const std::string& message,
        const std::string& signature,
        const std::string& sigAlg,
        const std::string& key
    );
    
}