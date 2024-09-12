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

std::string HSM::sign(const std::string& message, const std::string& type, const std::string& key)
{
    if(type.find("ECC") != std::string::npos) return ECC::sign(message, type, key);
    if(type.find("RSA") != std::string::npos) return RSA::sign(message, type, key);
    if(type.find("ECC") != std::string::npos) return ECC::sign(message, type, key);
    if(type.find("AES") != std::string::npos) return AES::sign(message, type, key);
    if(type.find("SHA2") != std::string::npos) return SHA2::sign(message, type, key);
    if(type.find("SHA3") != std::string::npos) return SHA3::sign(message, type, key);
    return std::string();
}

bool HSM::verify(const std::string& message, const std::string& signature, const std::string& type, const std::string& key)
{
    if(type.find("ECC") != std::string::npos) return ECC::verify(message, signature, type, key);
    if(type.find("RSA") != std::string::npos) return RSA::verify(message, signature, type, key);
    if(type.find("ECC") != std::string::npos) return ECC::verify(message, signature, type, key);
    if(type.find("AES") != std::string::npos) return AES::verify(message, signature, type, key);
    if(type.find("SHA2") != std::string::npos) return SHA2::verify(message, signature, type, key);
    if(type.find("SHA3") != std::string::npos) return SHA3::verify(message, signature, type, key);
    return false;
}
