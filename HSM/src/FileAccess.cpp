#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// // Function to encrypt using XOR
std::vector<unsigned char> xorEncryptDecrypt(const std::vector<unsigned char>& data, unsigned char key) {
    std::vector<unsigned char> output(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        output[i] = data[i] ^ key;
    }
    return output;
}

// Function to save an encrypted key in a file
void saveEncryptedKeyToFile(const std::string& key, const std::string& filename, unsigned char xorKey) {
// Encrypt the key
    std::vector<unsigned char> keyData(key.begin(), key.end());
    std::vector<unsigned char> encryptedData = xorEncryptDecrypt(keyData, xorKey);

// Saving the encrypted key to a file
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    file.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
    file.close();
}

// function to read an encrypted key from the file
std::string readEncryptedKeyFromFile(const std::string& filename, unsigned char xorKey) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return "";
    }

// Determining the file size
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

// Reading the encrypted data
    std::vector<unsigned char> encryptedData(fileSize);
    file.read(reinterpret_cast<char*>(encryptedData.data()), fileSize);
    file.close();

// reverse encryption (XOR) to recover the original key
    std::vector<unsigned char> decryptedData = xorEncryptDecrypt(encryptedData, xorKey);
    return std::string(decryptedData.begin(), decryptedData.end());
}

// int main() {
//     std::string key = "my_secret_key";
//     std::string filename = "keyfile.enc";
//     unsigned char xorKey = 0x5A;  // Example XOR key

//     saveEncryptedKeyToFile(key, filename, xorKey);

//     std::string decryptedKey = readEncryptedKeyFromFile(filename, xorKey);
//     std::cout << "Decrypted key: " << decryptedKey << std::endl;

//     return 0;
// }
