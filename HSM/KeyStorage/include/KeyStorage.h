#pragma once
#include "HSM_status_enum.cpp"
#include "HSM_type_enum.cpp"
#include <vector>
#include <string>

class KeyStorage
{
public:
    static HSM_STATUS get_keys(const std::vector<u_char>& myId, u_int32_t &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits = 512 );

protected:
    static HSM_STATUS getKeyFromKeyStorage(const std::vector<u_char> &myId, u_int32_t keyId, ENCRYPTION_ALGORITHM_TYPE type, std::vector<u_char> &publicKey, std::vector<u_char> &privateKey);

private:
    static std::string KeyStorageFileName = "KeyStorage.csv";

};
