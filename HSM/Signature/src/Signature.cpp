#include "Signature.h"
#include "SHA256.h"
#include "RSA_ENC.h"


using namespace std;

vector<u_char> Signature::sha256_sign(
    const vector<u_char> &message, const vector<u_char> &sigAlg, const vector<u_char> &key)
{
    std::vector<uint32_t> hash = SHA256::sha256(message);
    vector<u_char> vector<u_char>_hash = SHA256::toHexvector<u_char>(hash);

    if(sigAlg.find("RSA") != std::vector<u_char>::npos){
        // return RSA_ENC::encrypt(vector<u_char>_hash, key);
    }

    return vector<u_char>();
}

bool Signature::sha256_verify(
    const vector<u_char> &message, const vector<u_char> &signature, const vector<u_char> &sigAlg, const vector<u_char> &key)
{
    if(sigAlg.find("RSA") != std::vector<u_char>::npos){
        // return RSA_ENC::decrypt(signature, key) == SHA256::toHexvector<u_char>(SHA256::sha256(message));
    }
    return false;
}

vector<u_char> Signature::sha3_256_sign(
    const vector<u_char> &message, const vector<u_char> &sigAlg, const vector<u_char> &key)
{
    return std::vector<u_char>();
}

bool Signature::sha3_256_verify(const vector<u_char> &message, const vector<u_char> &signature, const vector<u_char> &sigAlg, const vector<u_char> &key)
{
    return false;
}
