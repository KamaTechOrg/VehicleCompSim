#include <string>
#include <gtest/gtest.h>

#include "HSM.h"
// #include "AES_API.hpp"

// TEST(AesTest, Aes_128_ecb_encrypt_decrypt_string_with_size_16) {
//   std::string key ="\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c";
//   std::string msg = "hello hello hsm!";
//   std::string expected_encrypted_msg =
//       "\xc2\x9e\x26\xc2\x8e\xdc\x25\x33\x44\x28\xe6\xa3\xe2\x72\x2d\xd2\xa2\x54"
//       "\xbe\x88\xe0\x37\xdd\xd9\xd7\x9f\xb6\x41\x1c\x3f\x9d\xf8";
//   EXPECT_EQ(AES::encrypt("AES-128-ECB", key, msg), expected_encrypted_msg);
//   EXPECT_EQ(AES::decrypt("AES-128-ECB", key, expected_encrypted_msg), msg);
// }

// TEST(API_TEST, write_end_read_test)
// {
//     HSM::KeyStorage& keyStorage = HSM::KeyStorage::getInstance();
//     keyStorage.writeToStorage();
//     keyStorage.searchInStorage("Emma");
//     keyStorage.writeToStorage("jjjj");
//     keyStorage.searchInStorage("jjjj");
// }

TEST(API_TEST, get_kye_rsa)
{
    HSM::KeyStorage &keyStorage = HSM::KeyStorage::getInstance();
    std::vector<u_char> myId = {'y', 'm'};
    u_int32_t keyId = 0;
    HSM::HSM_STATUS status = keyStorage.get_keys(myId, keyId, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, 512);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = HSM::Algo::encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    // std::vector<u_char> decrypted_massage;
    // status = HSM::Algo::decrypt(encrypted_massage, decrypted_massage, HSM::ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    // EXPECT_EQ(status, HSM::HSM_STATUS::HSM_Good);
    // EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
}