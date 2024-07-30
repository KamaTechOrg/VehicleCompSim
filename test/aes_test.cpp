#include "aes.hpp"
#include <array>
#include <cstdint>
#include <gtest/gtest.h>

static std::array<std::array<uint8_t, 4>, 4>
strToStateBlock(std::string const &str_block) {
  std::array<std::array<uint8_t, 4>, 4> state;
  assert(str_block.size() == 16);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      state[j][i] = str_block[i * 4 + j];
    }
  }
  return state;
}

TEST(AesTest, Aes_128_ecb_encrypt_decrypt_block) {
  // The test case below is taken from:
  // https://csrc.nist.gov/files/pubs/fips/197/final/docs/fips-197.pdf
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};
  std::string str_block =
      "\x32\x43\xf6\xa8\x88\x5a\x30\x8d\x31\x31\x98\xa2\xe0\x37\x07\x34";
  std::string encrypted_str_block =
      "\x39\x25\x84\x1d\x02\xdc\x09\xfb\xdc\x11\x85\x97\x19\x6a\x0b\x32";
  auto state = strToStateBlock(str_block);
  auto expected_encrypted_block = strToStateBlock(encrypted_str_block);
  Aes<AesVariant::Aes128> aes(key);
  auto encrypted_block = state;
  aes.encrypt(encrypted_block);
  EXPECT_EQ(encrypted_block, expected_encrypted_block);

  auto &expected_decrypted_block = state;
  auto &decrypted_block = expected_encrypted_block;

  aes.decrypt(decrypted_block);
  EXPECT_EQ(decrypted_block, expected_decrypted_block);
}

TEST(AesTest, Aes_128_ecb_encrypt_decrypt_string_with_size_16) {
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};
  std::string msg = "hello hello hsm!";
  std::string expected_encrypted_msg =
      "\xc2\x9e\x26\xc2\x8e\xdc\x25\x33\x44\x28\xe6\xa3\xe2\x72\x2d\xd2\xa2\x54"
      "\xbe\x88\xe0\x37\xdd\xd9\xd7\x9f\xb6\x41\x1c\x3f\x9d\xf8";
  Aes<AesVariant::Aes128> aes(key);
  EXPECT_EQ(aes.encrypt_ecb(msg), expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt_ecb(expected_encrypted_msg), msg);
}

TEST(AesTest, Aes_128_ecb_encrypt_decrypt_string_with_size_15) {
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};
  std::string msg = "hello hello hsm";
  std::string expected_encrypted_msg =
      "\xbb\x6e\xd0\xed\x30\xfa\x44\x64\xd2\x1f\xf1\xcb\xd9\x27\xf9\x56";
  Aes<AesVariant::Aes128> aes(key);
  auto result = aes.encrypt_ecb(msg);

  EXPECT_EQ(result.size(), expected_encrypted_msg.size());

  EXPECT_EQ(result, expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt_ecb(expected_encrypted_msg), msg);
}

TEST(AesTest, Aes_128_ecb_encrypt_decrypt_empty_string) {
  std::array<uint8_t, 16> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};
  std::string msg = "";
  std::string expected_encrypted_msg =
      "\xa2\x54\xbe\x88\xe0\x37\xdd\xd9\xd7\x9f\xb6\x41\x1c\x3f\x9d\xf8";
  Aes<AesVariant::Aes128> aes(key);
  auto result = aes.encrypt_ecb(msg);

  EXPECT_EQ(result, expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt_ecb(expected_encrypted_msg), msg);
}

TEST(AesTest, Aes_256_ecb_encrypt_decrypt_I) {
  std::array<uint8_t, 32> key = {
      0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
      0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
      0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60};
  std::string msg = "hello hello hsm";
  std::string expected_encrypted_msg =
      "\x23\x80\xEB\x34\xD4\xBE\x2F\x79\x46\x38\x64\xD4\x2F\x46\x41\xF8";
  Aes<AesVariant::Aes256> aes(key);
  auto result = aes.encrypt_ecb(msg);

  EXPECT_EQ(result, expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt_ecb(expected_encrypted_msg), msg);
}

TEST(AesTest, Aes_256_ecb_encrypt_decrypt_II) {
  std::array<uint8_t, 32> key = {
      0xbb, 0x85, 0x57, 0x2b, 0x2b, 0x82, 0xce, 0xc0, 0x93, 0x3b, 0xa9,
      0x38, 0x55, 0x77, 0x9b, 0xd3, 0xd4, 0xca, 0xd5, 0x93, 0x5f, 0xc0,
      0x84, 0x85, 0x8d, 0xc3, 0x71, 0xb0, 0x65, 0x4a, 0x60, 0x31};
  std::string msg = "Advanced Encryption Standard";
  std::string expected_encrypted_msg =
      "\x0b\x16\xb2\x5c\xbc\xf2\xd2\x91\xd8\x86\xf3\xbb\x91\x5e\xc3\xf6\x1c\xfa"
      "\xbc\xce\xab\x9e\xe8\xdd\x92\x6d\x68\x23\x96\x40\x51\x3c";
  Aes<AesVariant::Aes256> aes(key);
  auto result = aes.encrypt_ecb(msg);

  EXPECT_EQ(result, expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt_ecb(expected_encrypted_msg), msg);
}

TEST(AesTest, Aes_128_cbc_encrypt_decrypt_string_with_size_16) {
/*
  this test case based on Case #1 in:
  https://datatracker.ietf.org/doc/html/rfc3602#ref-ESP 
  but with padding.
*/
  std::array<uint8_t, 16> key = {0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b, 0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06};
  std::array<uint8_t, 16> iv = {0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30, 0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41};
  Aes<AesVariant::Aes128> aes(key);
  std::string msg = "Single block msg";
  std::string expected_encrypted_msg = 
    "\xe3\x53\x77\x9c\x10\x79\xae\xb8\x27\x08\x94\x2d\xbe\x77\x18\x1a"
    "\xb9\x7c\x82\x5e\x1c\x78\x51\x46\x54\x2d\x39\x69\x41\xbc\xe5\x5d";
  
  EXPECT_EQ(aes.encrypt_cbc(msg, iv), expected_encrypted_msg);
  EXPECT_EQ(aes.decrypt_cbc(expected_encrypted_msg, iv), msg);
}
