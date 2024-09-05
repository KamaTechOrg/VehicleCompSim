#pragma once
#include <string>
#include <vector>


// Signature class template definition
template <typename Hash, typename Encryption>
class Signature {
public:
    static std::string sign(const std::string& data, const std::vector<std::string> &params) {
        std::string hashedData = Hash::hash(data);
        std::string signature = Encryption::encrypt(hashedData, params);
        return signature;
    }

	static bool verify(const std::string& data, const std::string& signature, const std::vector<std::string> &params) {
		std::string hashedData = Hash::hash(data);
		std::string decryptedSignature = Encryption::decrypt(signature, params);
		return hashedData == decryptedSignature;
	}
};

    // Using the Signature class template with SHA256 and RSA
    //std::string rsaSignature = Signature<SHA256, RSA>::sign("hello", { "modulus", "privateKey" });
