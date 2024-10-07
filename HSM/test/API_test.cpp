#include <string>
#include <gtest/gtest.h>
#include "HSM.h"
#include "AES_API.hpp"

// TEST(AesTest, Aes_128_ecb_encrypt_decrypt_string_with_size_16) {
//   std::string key ="\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c";
//   std::string msg = "hello hello hsm!";
//   std::string expected_encrypted_msg =
//       "\xc2\x9e\x26\xc2\x8e\xdc\x25\x33\x44\x28\xe6\xa3\xe2\x72\x2d\xd2\xa2\x54"
//       "\xbe\x88\xe0\x37\xdd\xd9\xd7\x9f\xb6\x41\x1c\x3f\x9d\xf8";
//   EXPECT_EQ(AES::encrypt("AES-128-ECB", key, msg), expected_encrypted_msg);
//   EXPECT_EQ(AES::decrypt("AES-128-ECB", key, expected_encrypted_msg), msg);
// }

TEST(API_TEST, get_ID)
{
    HSM::Ident myId = HSM::Ident();
    std::string str = myId.toString();
    std::cout << "myId:" << str << std::endl;
    EXPECT_TRUE(str.length() > 0);
}

TEST(API_TEST, get_kye_rsa_64_bit)
{
    HSM::KeyStorage &keyStorage = HSM::KeyStorage::getInstance();
    HSM::Ident myId = HSM::Ident();
    HSM::KeyId keyId;
    HSM::HSM_STATUS status = keyStorage.get_keys(myId, keyId, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, 64);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = HSM::Algo::encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = HSM::Algo::decrypt(encrypted_massage, decrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
    HSM::KeyStorage::resetInstance();
}
TEST(API_TEST, get_kye_rsa_128_bit)
{
    HSM::KeyStorage &keyStorage = HSM::KeyStorage::getInstance();
    HSM::Ident myId = HSM::Ident();
    HSM::KeyId keyId;
    HSM::HSM_STATUS status = keyStorage.get_keys(myId, keyId, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, 128);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = HSM::Algo::encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = HSM::Algo::decrypt(encrypted_massage, decrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
    HSM::KeyStorage::resetInstance();
}

TEST(API_TEST, rsa_encrypt_without_user_id)
{
    HSM::KeyStorage &keyStorage = HSM::KeyStorage::getInstance();
    HSM::KeyId keyId;
    HSM::Ident myId = HSM::Ident();
    HSM::HSM_STATUS status = keyStorage.get_keys(myId, keyId, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, 64);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = HSM::Algo::encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, HSM::Ident{"notMyId"}, keyId, false);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = HSM::Algo::decrypt(encrypted_massage, decrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
    HSM::KeyStorage::resetInstance();
}

TEST(API_TEST, get_kye_invalid_type)
{
    HSM::KeyStorage &keyStorage = HSM::KeyStorage::getInstance();
    HSM::Ident myId = HSM::Ident();
    HSM::KeyId keyId;
    HSM::HSM_STATUS status = keyStorage.get_keys(myId, keyId, HSM::ENCRYPTION_ALGORITHM_TYPE::NoAlg, 128);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_InvalidAlg);
}

TEST(API_AES, auto_generated)
{
    std::array<HSM::ENCRYPTION_ALGORITHM_TYPE, 9> aes_types = {
      ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB,
      ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC,
      ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR,
      ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB,
      ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC,
      ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR,     
      ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB,
      ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC,
      ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR
    };

    std::vector<size_t> msg_sizes {0, 1, 2, 15, 16, 17, 31, 32, 33, 1234, 5678};
    srand(0);

    for(auto msg_size: msg_sizes){
        std::vector<uint8_t> msg(msg_size);
        for(auto& c: msg) c=rand();
        
        for(ENCRYPTION_ALGORITHM_TYPE aes_type: aes_types){
            std::vector<uint8_t> key;
            std::string status = "fail on aes_type: " + std::to_string(aes_type) + ",  msg_size: " + std::to_string(msg_size);
            ASSERT_EQ(AES::generateKey(key, aes_type), HSM_STATUS::HSM_Good) << status;
            std::vector<uint8_t> enc_msg;
            std::vector<uint8_t> clr_msg;
            ASSERT_EQ(AES::encrypt(key, msg, enc_msg, aes_type), HSM_STATUS::HSM_Good) << status;
            ASSERT_EQ(AES::decrypt(key, enc_msg, clr_msg, aes_type), HSM_STATUS::HSM_Good) << status;
            EXPECT_EQ(msg, clr_msg) << status;
        }
    }

}

TEST(API_AES, printAESKye)
{
    ENCRYPTION_ALGORITHM_TYPE aes_type = ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB;
    std::vector<uint8_t> key;
    EXPECT_EQ(AES::generateAndPrintKey(key, aes_type), HSM_STATUS::HSM_Good);
}