#include <string>
#include "HSM.h"
#include "AES_API.hpp"
#include "HSM_type_enum.hpp"

std::pair<std::vector<u_char>, std::vector<u_char>> HSM::generateAsymmetricKeys(ENCRYPTION_ALGORITHM_TYPE type, int bits = 512)
{
    if(type == ENCRYPTION_ALGORITHM_TYPE::RSA) return RSA::generateKeys(type, bits);
    // if(type == ENCRYPTION_ALGORITHM_TYPE::ECC) return ECC::generateKeys(type, bits);
    return std::pair<std::vector<u_char>, std::vector<u_char>> ();
}
std::vector<u_char> HSM::generateSymmetricKey(ENCRYPTION_ALGORITHM_TYPE  type, int bits = 128)
{
    // if(type == ENCRYPTION_ALGORITHM_TYPE::AES) return AES::generateKey(type);
    return std::vector<u_char>();
}

std::vector<u_char> HSM::encrypt(const std::vector<u_char>& message, ENCRYPTION_ALGORITHM_TYPE type,const std::vector<u_char>& key , int bits = 512)
{
    if(type == ENCRYPTION_ALGORITHM_TYPE::AES) return AES::encrypt(message, key, bits);
    if(type == ENCRYPTION_ALGORITHM_TYPE::RSA) return RSA::encrypt(message, key);
    // if(type.find("ECC") != std::vector<u_char>::npos) return ECC::encrypt(message, key);
    return std::vector<u_char>();
}

std::vector<u_char> HSM::decrypt(const std::vector<u_char>& message, ENCRYPTION_ALGORITHM_TYPE type,const std::vector<u_char>& key, int bits = 512)
{
    if(type == ENCRYPTION_ALGORITHM_TYPE::AES) return AES::decrypt(type, message, key);
    if(type == ENCRYPTION_ALGORITHM_TYPE::RSA) return RSA::decrypt(message, key, bits);
    // if(type.find("ECC") != std::vector<u_char>::npos) return ECC::decrypt(message, key);
    return std::vector<u_char>();
}

std::vector<u_char> HSM::signMessage(
        const std::vector<u_char>& message, 
        ENCRYPTION_ALGORITHM_TYPE sigAlg, 
        ENCRYPTION_ALGORITHM_TYPE hashAlg,
        const std::vector<u_char>& key
){
    // if(hashAlg.find("SHA3_256") != std::vector<u_char>::npos) return Signature::sha3_256_sign(message, sigAlg, key);
    if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA256) return Signature::sha256_sign(message, sigAlg, key);
    return std::vector<u_char>();
}

bool HSM::verify(
        const std::vector<u_char>& message,
        const std::vector<u_char>& signature,
        ENCRYPTION_ALGORITHM_TYPE sigAlg,
        ENCRYPTION_ALGORITHM_TYPE hashAlg,
        const std::vector<u_char>& key
){
    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA3_256) return Signature::sha3_256_verify(message, sigAlg, key);
    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA256) return Signature::sha256_verify(message, sigAlg, key);
    return false;
}
