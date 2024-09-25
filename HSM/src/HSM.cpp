#include "HSM.h"
#include "AES_API.hpp"
#include "RSA_ENC.h"
#include "RSA_KEY.h"
#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace HSM;

std::string vectorToString(std::vector<u_char> vec)
{
    std::stringstream ss;
    for (auto c : vec)
    {
        ss << c;
    }
    return ss.str();
}

std::vector<std::vector<u_char>> keyStingToVector(std::string str)
{

    std::vector<std::vector<u_char>> vec;
    std::stringstream ss(str);
    std::string word;
    while (std::getline(ss, word, ','))
    {
        std::stringstream ss2(word);
        std::string word2 = ss2.str();
        std::vector<u_char> v;
        for(u_char c: word2){
            v.emplace_back(c);
        }
        vec.push_back(v);
    }

    return vec;
}

const std::string KeyStorage::KeyStorageFileName = "KeyStorage.csv";
KeyStorage *KeyStorage::instance = nullptr;

KeyStorage::KeyStorage()
{

    std::cout << "KeyStorage constructor" << std::endl;
    //create file if not exists
    std::ofstream file(KeyStorageFileName, std::ios_base::app);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file for writing\n";
        return;
    }
    file.close();
}

HSM::KeyStorage::~KeyStorage()
{
    //erase file
    std::remove(KeyStorageFileName.c_str());
    // delete instance; 
    // KeyStorage::instance = nullptr;
}

HSM_STATUS HSM::KeyStorage::writeToStorage(std::string info)
{
    std::ofstream file(KeyStorageFileName, std::ios_base::app);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file for writing\n";
        return HSM_STATUS::HSM_InternalErr;
    }

    file << info + "\n";

    file.close(); // Close the file after writing
    return HSM_STATUS::HSM_Good;
}

HSM_STATUS HSM::KeyStorage::searchInStorage(const Ident &myId, u_int32_t &keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey)
{
    std::ifstream file(KeyStorageFileName);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file for reading\n";
        return HSM_STATUS::HSM_InternalErr;
    }

    std::string line, word;

    // Reading file line by line
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        //convert line to vector string split by comma
        std::vector<std::string> vec;
        while (std::getline(ss, word, ','))
        {
            vec.push_back(word);
        }
        Ident IdCheck = Ident(vec[0]);
        if (IdCheck.compareID(myId) == HSM_STATUS::HSM_Good && vec[1] == std::to_string(keyId) && vec[2] == std::to_string(type))
        {
            std::cout << "Found: " << line << "\n";
            file.close();
            std::vector<std::vector<u_char>> vec = keyStingToVector(line);
            publicKey = vec[3];
            privateKey = vec[4];
            file.close();
            return HSM_STATUS::HSM_Good;
        }
    }
    file.close();
    return HSM_STATUS::HSM_NoSuchKey;
}

HSM_STATUS KeyStorage::get_keys(const Ident &myId, u_int32_t &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = HSM_STATUS();
    switch (type)
    {
    case RSA:

        status = RSA_KEY::generateKeys(publicKey, privateKey, bits);

        break;

    case ECC:

        break;

    case AES:
        break;

    default:
        return HSM_STATUS::HSM_InvalidAlg;
    }

    if (status == HSM_STATUS::HSM_Good)
    {
        keyId = KeyStorage::getInstance().keyIdCounter++;
        std::stringstream ss;
        ss << myId.toString() << "," << keyId << "," << type << "," << vectorToString(publicKey) << "," << vectorToString(privateKey);
        status = KeyStorage::getInstance().writeToStorage(ss.str());
    }
    return status;
}

HSM::KeyStorage &HSM::KeyStorage::getInstance()
{
    if (instance == nullptr)
        instance = new KeyStorage();
    return *instance;
}

HSM_STATUS KeyStorage::getKeyFromKeyStorage(const Ident &myId, u_int32_t keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey)
{
    HSM_STATUS status = HSM_STATUS();
    status = KeyStorage::getInstance().searchInStorage(myId, keyId, type, publicKey, privateKey);
    return status;
}

HSM_STATUS Algo::encrypt(const std::vector<u_char> &message, std::vector<u_char> &encrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const Ident &myId, u_int32_t keyId)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = KeyStorage::getInstance().getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;

    switch (type)
    {
    case RSA:
        encrypted_message = RSA_ENC::encrypt(message, publicKey);

        break;

    case AES:
        encrypted_message = AES::encrypt(message, publicKey);
        break;

    default:
        return HSM_STATUS();
    }
    return HSM_STATUS::HSM_Good;
}

HSM_STATUS Algo::decrypt(const std::vector<u_char> &message, std::vector<u_char> &decrypted_message, ENCRYPTION_ALGORITHM_TYPE type, const Ident &myId, u_int32_t keyId)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = KeyStorage::getInstance().getKeyFromKeyStorage(myId, keyId, type, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;

    switch (type)
    {
    case RSA:
        decrypted_message = RSA_ENC::decrypt(message, privateKey);
        break;
    case AES:
        decrypted_message = AES::decrypt(message, privateKey);
        break;

    default:
        break;
    }
    return HSM_STATUS();
}

HSM_STATUS Algo::signMessage(const std::vector<u_char> &message, std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const Ident &myIdForSign, u_int32_t keyIdForSign, const Ident &myIdForHash, u_int32_t keyIdForHash)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;

    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA3_256) return Signature::sha3_256_sign(message, sigAlg, key);
    // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA256) return Signature::sha256_sign(message, sigAlg, key);
    return HSM_STATUS();
}

HSM_STATUS Algo::verify(const std::vector<u_char> &message, const std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const Ident &myIdForSign, u_int32_t keyIdForSign, const Ident &myIdForHash, u_int32_t keyIdForHash)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
    HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
    if (status != HSM_STATUS::HSM_Good)
        return status;

    return HSM_STATUS();
}

// HSM_STATUS Algo::signMessage(const std::vector<u_char> &message, std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign, const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash)
// {
//     std::vector<u_char> publicKey;
//     std::vector<u_char> privateKey;
//     HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
//     if (status != HSM_STATUS::HSM_Good)
//         return status;

//     // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA3_256) return Signature::sha3_256_sign(message, sigAlg, key);
//     // if(hashAlg == ENCRYPTION_ALGORITHM_TYPE::SHA256) return Signature::sha256_sign(message, sigAlg, key);
//     return HSM_STATUS();
// }

// HSM_STATUS Algo::verify(const std::vector<u_char> &message, const std::vector<u_char> &signature, ENCRYPTION_ALGORITHM_TYPE sigAlg, ENCRYPTION_ALGORITHM_TYPE hashAlg, const std::vector<u_char> &myIdForSign, u_int32_t keyIdForSign, const std::vector<u_char> &myIdForHash, u_int32_t keyIdForHash)
// {
//     std::vector<u_char> publicKey;
//     std::vector<u_char> privateKey;
//     HSM_STATUS status = getKeyFromKeyStorage(myIdForSign, keyIdForSign, sigAlg, publicKey, privateKey);
//     if (status != HSM_STATUS::HSM_Good)
//         return status;

//     return HSM_STATUS();
// }
