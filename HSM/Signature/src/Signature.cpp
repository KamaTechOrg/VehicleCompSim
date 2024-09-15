#include "Signature.h"
#include "SHA256.h"

using namespace std;

string Signature::sha256_sign(
    const string &message, const string &sigAlg, const string &key)
{
    std::vector<uint32_t> hash = SHA256::sha256(message);
    string string_hash = SHA256::toHexString(hash);

    if(sigAlg.find("RSA") != std::string::npos){
        return 
    }


    return string();
}

bool Signature::sha256_verify(
    const string &message, const string &signature, const string &sigAlg, const string &key)
{
    return false;
}

string Signature::sha3_256_sign(
    const string &message, const string &sigAlg, const string &key)
{
    return std::string();
}

bool Signature::sha3_256_verify(const string &message, const string &signature, const string &sigAlg, const string &key)
{
    return false;
}
