#include <string>
#include <gtest/gtest.h>
#include "HSM.h"
#include "AES_API.hpp"

using namespace HSMns;

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
    Ident myId = Ident();
    std::string str = myId.toString();
    std::cout << "myId:" << str << std::endl;
    EXPECT_TRUE(str.length() > 0);
}

TEST(API_TEST, get_kye_rsa_64_bit)
{
    HSM &hsm = getInstance();
    Ident myId = Ident();
    KeyId keyId;
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::RSA, 64);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = hsm.encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = hsm.decrypt(encrypted_massage, decrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
    HSM::resetInstance();
}
TEST(API_TEST, get_kye_rsa_128_bit)
{
    HSM &hsm = getInstance();
    Ident myId = Ident();
    KeyId keyId;
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::RSA, 128);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = hsm.encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = hsm.decrypt(encrypted_massage, decrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
    HSM::resetInstance();
}

TEST(API_TEST, rsa_encrypt_without_user_id)
{
    HSM &hsm = getInstance();
    KeyId keyId;
    Ident myId = Ident();
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::RSA, 64);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = hsm.encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, Ident{"notMyId"}, keyId, false);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = hsm.decrypt(encrypted_massage, decrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
    HSM::resetInstance();
}

TEST(API_TEST, get_kye_invalid_type)
{
    HSM &hsm = getInstance();
    Ident myId = Ident();
    KeyId keyId;
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::NoAlg, 128);
    EXPECT_EQ(status, HSM_STATUS::HSM_InvalidAlg);
}

TEST(API_TEST, aes_all_types_many_msg_sizes)
{
    std::array<ENCRYPTION_ALGORITHM_TYPE, 9> aes_types = {
        ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB,
        ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC,
        ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR,
        ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB,
        ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC,
        ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR,
        ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB,
        ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC,
        ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR};

    std::vector<size_t> msg_sizes{0, 1, 2, 15, 16, 17, 31, 32, 33, 1234, 56789, 101112};
    srand(0);

    for (auto msg_size : msg_sizes)
    {
        std::vector<uint8_t> msg(msg_size);
        for (auto &c : msg)
            c = rand();

        for (ENCRYPTION_ALGORITHM_TYPE aes_type : aes_types)
        {
            std::string failure_msg = "fail on aes_type: " + std::to_string(aes_type) + ",  msg_size: " + std::to_string(msg_size);
            HSM &hsm = getInstance();
            KeyId keyId;
            Ident myId;
            HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, aes_type);
            ASSERT_EQ(status, HSM_STATUS::HSM_Good) << failure_msg;

            std::vector<uint8_t> enc_msg;
            std::vector<uint8_t> clr_msg;
            ASSERT_EQ(hsm.encrypt(msg, enc_msg, aes_type, myId, keyId), HSM_STATUS::HSM_Good) << failure_msg;
            ASSERT_EQ(hsm.decrypt(enc_msg, clr_msg, aes_type, myId, keyId), HSM_STATUS::HSM_Good) << failure_msg;
            EXPECT_EQ(msg, clr_msg) << failure_msg;
            HSM::resetInstance();
        }
    }
}

TEST(API_AES, printAESKye)
{
    ENCRYPTION_ALGORITHM_TYPE aes_type = ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB;
    std::vector<uint8_t> key;
    EXPECT_EQ(AES::generateAndPrintKey(key, aes_type), HSM_STATUS::HSM_Good);
}