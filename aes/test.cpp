#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include "aes.hpp"

static std::array<std::array<uint8_t, 4>, 4> strToStateBlock(std::string const& str_block){
  std::array<std::array<uint8_t, 4>, 4> state;
  assert(str_block.size() == 16);
  for(int i = 0; i<4; ++i){
   for(int j = 0; j<4; ++j){
      state[j][i] = str_block[i*4 + j];
    }
  }
  return state;
}

TEST(AesTest, Aes_128_encrypt_decrypt_block) {
  //The test case below is taken from: https://csrc.nist.gov/files/pubs/fips/197/final/docs/fips-197.pdf
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab,
   0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  std::string str_block = "\x32\x43\xf6\xa8\x88\x5a\x30\x8d\x31\x31\x98\xa2\xe0\x37\x07\x34";
  std::string encrypted_str_block = "\x39\x25\x84\x1d\x02\xdc\x09\xfb\xdc\x11\x85\x97\x19\x6a\x0b\x32";
  auto state = strToStateBlock(str_block);
  auto expected_encrypted_block = strToStateBlock(encrypted_str_block);
  Aes<AesVariant::Aes128> aes(key);
  auto encrypted_block = state;
  aes.encrypt(encrypted_block);
  EXPECT_EQ(encrypted_block, expected_encrypted_block);

  auto& expected_decrypted_block = state;
  auto& decrypted_block = expected_encrypted_block;

  aes.decrypt(decrypted_block);
  EXPECT_EQ(decrypted_block, expected_decrypted_block);
}

TEST(AesTest, Aes_128_encrypt_decrypt_string_with_size_16) {
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab,
   0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  std::string msg = "hello hello hsm!";
  std::string expected_encrypted_msg = "\xc2\x9e\x26\xc2\x8e\xdc\x25\x33\x44\x28\xe6\xa3\xe2\x72\x2d\xd2\xa2\x54\xbe\x88\xe0\x37\xdd\xd9\xd7\x9f\xb6\x41\x1c\x3f\x9d\xf8";
  Aes<AesVariant::Aes128> aes(key);
  EXPECT_EQ(aes.encrypt(msg), expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt(expected_encrypted_msg), msg);
}

TEST(AesTest, Aes_128_encrypt_decrypt_string_with_size_15) {
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab,
   0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  std::string msg = "hello hello hsm";
  std::string expected_encrypted_msg = "\xbb\x6e\xd0\xed\x30\xfa\x44\x64\xd2\x1f\xf1\xcb\xd9\x27\xf9\x56";
  Aes<AesVariant::Aes128> aes(key);
  auto result = aes.encrypt(msg);

  EXPECT_EQ(result.size(), expected_encrypted_msg.size());

  EXPECT_EQ(result, expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt(expected_encrypted_msg), msg);
}

TEST(AesTest, Aes_128_encrypt_decrypt_empty_string) {
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab,
   0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  std::string msg = "";
  std::string expected_encrypted_msg = "\xa2\x54\xbe\x88\xe0\x37\xdd\xd9\xd7\x9f\xb6\x41\x1c\x3f\x9d\xf8";
  Aes<AesVariant::Aes128> aes(key);
  auto result = aes.encrypt(msg);

  EXPECT_EQ(result.size(), expected_encrypted_msg.size());

  EXPECT_EQ(result, expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt(expected_encrypted_msg), msg);
}
