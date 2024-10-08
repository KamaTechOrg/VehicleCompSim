#include "HSM.h"
#include "AES_API.hpp"
#include "RSA_KEY.h"
#include <ostream>
#include <sstream>

using namespace HSMns;

std::string vectorToString(std::vector<u_int8_t> vec)
{
    std::stringstream ss;
    for (auto c : vec)
    {
        ss << c;
    }
    return ss.str();
}

std::string binaryVectorToString(std::vector<u_int8_t> vec)
{
    std::stringstream ss;
    for (auto const &b : vec)
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)b;
    std::string tmp = ss.str();
    return ss.str();
}

std::vector<u_int8_t> stringToBinaryVector(std::string str)
{
    std::vector<u_int8_t> vec;
    for (int i = 0; i < str.size(); i += 2)
    {
        std::string c = str.substr(i, 2);
        int n = std::stoi(c, nullptr, 16);
        vec.emplace_back(n);
    }
    return vec;
}

std::vector<std::vector<u_int8_t>> keyStingToVector(std::string str)
{

    std::vector<std::vector<u_int8_t>> vec;
    std::stringstream ss(str);
    std::string word;
    int count = 0;
    while (std::getline(ss, word, ','))
    {
        std::stringstream ss2(word);
        std::string word2 = ss2.str();
        std::vector<u_int8_t> v;
        if (count == 4)
        {
            v = stringToBinaryVector(word2);
        }
        else
        {
            for (u_char c : word2)
            {
                v.emplace_back(c);
            }
        }
        vec.push_back(v);
        count++;
    }

    return vec;
}

const std::string KeyStorage::KeyStorageFileName = "KeyStorage.csv";

KeyStorage::KeyStorage(ENCRYPTION_ALGORITHM_TYPE kekAlgorithmType, std::string stringKeyForKek)
{

    // create file if not exists
    std::ofstream file(KeyStorageFileName, std::ios_base::app);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file for writing\n";
        return;
    }
    file.close();
    kekAlgType = kekAlgorithmType;
    keyForKek.resize(stringKeyForKek.size() / 2);
    for (int i = 0; i < keyForKek.size(); i += 2)
    {
        keyForKek[i] = stoi(stringKeyForKek.substr(i, 2), nullptr, 16);
    }
    
}

KeyStorage::~KeyStorage()
{
    // clear the csv file
    std::ofstream file(KeyStorageFileName, std::ios::trunc);
    file.close();
    // delete instance;
    // delete instance;
    // KeyStorage::instance = nullptr;
}


HSM_STATUS KeyStorage::writeToStorage(std::string info)
{
    std::ofstream file(KeyStorageFileName, std::ios_base::app);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file for writing\n";
        return HSM_STATUS::HSM_InternalErr;
    }

    file << info + "\n";

    return HSM_STATUS::HSM_Good;
}

HSM_STATUS KeyStorage::getKeyFromKeyStorage(const Ident &myId, const KeyId &keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_int8_t> &publicKey, std::vector<u_int8_t> &privateKey, bool needPrivilege) const
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
        // convert line to vector string split by comma
        std::vector<std::string> vec;
        while (std::getline(ss, word, ','))
        {
            vec.push_back(word);
        }
        Ident IdCheck = Ident(vec[0]);
        if (IdCheck.compareID(myId) == HSM_STATUS::HSM_Good && vec[1] == std::to_string(keyId) && vec[2] == std::to_string(type))
        {
            // std::cout << "Found: " << line << "\n";
            file.close();
            std::vector<std::vector<u_int8_t>> vec = keyStingToVector(line);
            publicKey = vec[3];
            std::vector<u_int8_t> encryptedPrivateKey = vec[4];
            // file.close();
            HSM_STATUS status = AES::decrypt(encryptedPrivateKey, privateKey, keyForKek, kekAlgType);
            if (status != HSM_STATUS::HSM_Good)
                return status;
            return HSM_STATUS::HSM_Good;
        }
        else if (vec[1] == std::to_string(keyId) && vec[2] == std::to_string(type) && !needPrivilege)
        {
            // std::cout << "Found !needPrivilege: " << line << "\n";
            file.close();
            std::vector<std::vector<u_int8_t>> vec = keyStingToVector(line);
            publicKey = vec[3];
            file.close();
            return HSM_STATUS::HSM_Good;
        }
    }
    file.close();
    return HSM_STATUS::HSM_NoSuchKey;
}

HSM_STATUS KeyStorage::create_key_and_get_id(const Ident &myId, KeyId &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
    std::vector<u_int8_t> publicKey;
    std::vector<u_int8_t> privateKey;
    HSM_STATUS status = HSM_STATUS();
    switch (type)
    {
    case RSA:
        status = RSA_KEY::generateKeys(publicKey, privateKey, bits);
        break;

        // case ECC:
        //     break;

    case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR:
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC:
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR:
        status = AES::generateKey(privateKey, type);
        break;

    default:
        return HSM_STATUS::HSM_InvalidAlg;
    }

    if (status == HSM_STATUS::HSM_Good)
    {
        keyId = keyIdCounter++;
        std::stringstream ss;
        // encrypt private key using kek
        std::vector<u_int8_t> encryptedPrivateKey;
        status = AES::encrypt(privateKey, encryptedPrivateKey, keyForKek, kekAlgType);
        ss << myId.toString() << "," << keyId << "," << type << "," << vectorToString(publicKey) << "," << binaryVectorToString(encryptedPrivateKey);
        std::string tmp = ss.str();
        status = writeToStorage(ss.str());
    }
    return status;
}



