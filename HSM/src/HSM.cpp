#include "HSM.h"

std::pair<std::string, std::string> HSM::generateAsymmetricKeys(const std::string& type)
{
    if(type.find("RSA") != std::string::npos) return RSA::generateKeys(type);
    if(type.find("ECC") != std::string::npos) return ECC::generateKeys(type);
    return std::pair<std::string, std::string> ();
}
std::string HSM::generateSymmetricKey(const std::string& type)
{
    if(type.find("AES") != std::string::npos) return AES::generateKey(type);
    return std::string();
}

std::string HSM::encrypt(const std::string& message, const std::string& type,const std::string& key)
{
    if(type.find("AES") != std::string::npos) return AES::encrypt(message, key);
    if(type.find("RSA") != std::string::npos) return RSA::encrypt(message, key);
    if(type.find("ECC") != std::string::npos) return ECC::encrypt(message, key);
    return std::string();
}

std::string HSM::decrypt(const std::string& message, const std::string& type,const std::string& key)
{
    if(type.find("AES") != std::string::npos) return AES::decrypt(message, key);
    if(type.find("RSA") != std::string::npos) return RSA::decrypt(message, key);
    if(type.find("ECC") != std::string::npos) return ECC::decrypt(message, key);
    return std::string();
}

std::string HSM::signMessage(
        const std::string& message, 
        const std::string& sigAlg, 
        const std::string& hashAlg,
        const std::string& key
){
    if(hashAlg.find("SHA3_256") != std::string::npos) return Signature::sha3_256_sign(message, sigAlg, key);
    if(hashAlg.find("SHA256") != std::string::npos) return Signature::sha256_sign(message, sigAlg, key);
    return std::string();
}

bool HSM::verify(
        const std::string& message,
        const std::string& signature,
        const std::string& sigAlg,
        const std::string& hashAlg,
        const std::string& key
){
    if(hashAlg.find("SHA3_256") != std::string::npos) return Signature::sha3_256_verify(message, sigAlg, key);
    if(hashAlg.find("SHA256") != std::string::npos) return Signature::sha256_verify(message, sigAlg, key);
    return false;
}
