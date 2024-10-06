#include "HSM.h"
#include "AES_API.hpp"
#include "RSA_KEY.h"
#include <ostream>
#include <sstream>
#include "KyeforKek.hpp"

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
        for (u_char c : word2)
        {
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
    // create file if not exists
    std::ofstream file(KeyStorageFileName, std::ios_base::app);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file for writing\n";
        return;
    }
    file.close();
    HSM_STATUS status;
    kekAlgType = ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB;
    // kekAlgType = kekAlgorithmType;
    // keyForKek = std::vector<u_char>(stringKeyForKek.begin(), stringKeyForKek.end());
    status = AES::generateKey(keyForKek, kekAlgType);
    if (status != HSM_STATUS::HSM_Good)
    {
        std::cerr << "Failed to generate key" << std::endl;
        return;
    }
}

HSM::KeyStorage::~KeyStorage()
{
    // erase file
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

HSM_STATUS HSM::KeyStorage::searchInStorage(const Ident &myId, const KeyId &keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey, bool needPrivilege)
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
            std::cout << "Found: " << line << "\n";
            file.close();
            std::vector<std::vector<u_char>> vec = keyStingToVector(line);
            publicKey = vec[3];
            std::vector<u_char> encryptedPrivateKey = vec[4];
            file.close();
            HSM_STATUS status = AES::decrypt(getInstance().keyForKek, encryptedPrivateKey, privateKey, getInstance().kekAlgType);
            if (status != HSM_STATUS::HSM_Good)
                return status;
            return HSM_STATUS::HSM_Good;
        }
        else if (vec[1] == std::to_string(keyId) && vec[2] == std::to_string(type) && !needPrivilege)
        {
            std::cout << "Found !needPrivilege: " << line << "\n";
            file.close();
            std::vector<std::vector<u_char>> vec = keyStingToVector(line);
            publicKey = vec[3];
            file.close();
            return HSM_STATUS::HSM_Good;
        }
    }
    file.close();
    return HSM_STATUS::HSM_NoSuchKey;
}

HSM_STATUS KeyStorage::get_keys(const Ident &myId, KeyId &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
    std::vector<u_char> publicKey;
    std::vector<u_char> privateKey;
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
        keyId = KeyStorage::getInstance().keyIdCounter++;
        std::stringstream ss;
        // encrypt private key using kek
        std::vector<u_char> encryptedPrivateKey;
        status = AES::encrypt(getInstance().keyForKek, privateKey, encryptedPrivateKey, getInstance().kekAlgType);
        ss << myId.toString() << "," << keyId << "," << type << "," << vectorToString(publicKey) << "," << vectorToString(encryptedPrivateKey);
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

HSM_STATUS KeyStorage::getKeyFromKeyStorage(const Ident &myId, const KeyId &keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey, bool needPrivilege)
{
    HSM_STATUS status = HSM_STATUS();
    status = KeyStorage::getInstance().searchInStorage(myId, keyId, type, publicKey, privateKey, needPrivilege);
    return status;
}