#include <string>
#include "HSM.h"
#include "AES_API.hpp"

std::pair<std::vector<u_char>, std::vector<u_char>> HSM::generateAsymmetricKeys(const std::vector<u_char>& type)
{
    if(type.find("RSA") != std::vector<u_char>::npos) return RSA::generateKeys(type);
    // if(type.find("ECC") != std::vector<u_char>::npos) return ECC::generateKeys(type);
    return std::pair<std::vector<u_char>, std::vector<u_char>> ();
}
std::vector<u_char> HSM::generateSymmetricKey(const std::vector<u_char>& type)
{
    // if(type.find("AES") != std::vector<u_char>::npos) return AES::generateKey(type);
    return std::vector<u_char>();
}

std::vector<u_char> HSM::encrypt(const std::vector<u_char>& message, const std::vector<u_char>& type,const std::vector<u_char>& key)
{
    if(type.find("AES") != std::vector<u_char>::npos) return AES::encrypt(type, message, key);
    if(type.find("RSA") != std::vector<u_char>::npos) return RSA::encrypt(message, key);
    // if(type.find("ECC") != std::vector<u_char>::npos) return ECC::encrypt(message, key);
    return std::vector<u_char>();
}

std::vector<u_char> HSM::decrypt(const std::vector<u_char>& message, const std::vector<u_char>& type,const std::vector<u_char>& key)
{
    if(type.find("AES") != std::vector<u_char>::npos) return AES::decrypt(type, message, key);
    if(type.find("RSA") != std::vector<u_char>::npos) return RSA::decrypt(message, key);
    // if(type.find("ECC") != std::vector<u_char>::npos) return ECC::decrypt(message, key);
    return std::vector<u_char>();
}

std::vector<u_char> HSM::signMessage(
        const std::vector<u_char>& message, 
        const std::vector<u_char>& sigAlg, 
        const std::vector<u_char>& hashAlg,
        const std::vector<u_char>& key
){
    // if(hashAlg.find("SHA3_256") != std::vector<u_char>::npos) return Signature::sha3_256_sign(message, sigAlg, key);
    if(hashAlg.find("SHA256") != std::vector<u_char>::npos) return Signature::sha256_sign(message, sigAlg, key);
    return std::vector<u_char>();
}

bool HSM::verify(
        const std::vector<u_char>& message,
        const std::vector<u_char>& signature,
        const std::vector<u_char>& sigAlg,
        const std::vector<u_char>& hashAlg,
        const std::vector<u_char>& key
){
    // if(hashAlg.find("SHA3_256") != std::vector<u_char>::npos) return Signature::sha3_256_verify(message, sigAlg, key);
    // if(hashAlg.find("SHA256") != std::vector<u_char>::npos) return Signature::sha256_verify(message, sigAlg, key);
    return false;
}
