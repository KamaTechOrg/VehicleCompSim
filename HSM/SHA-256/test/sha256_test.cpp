
#include <cassert>
#include <gtest/gtest.h>
#include "SHA-256.h"


TEST(SHA256_Test, Empty_message){
    std::string message1 = "";
    std::vector<uint32_t> expected_hash1 = {
        0xe3b0c442, 0x98fc1c14, 0x9afbf4c8, 0x996fb924, 0x27ae41e4, 0x649b934c, 0xa495991b, 0x7852b855
    };
    EXPECT_EQ(sha256(message1), expected_hash1);
}

TEST(SHA256_Test, Single_character){
    std::string message = "a";
    std::vector<uint32_t> expected_hash = {
        0xca978112, 0xca1bbdca, 0xfac231b3, 0x9a23dc4d, 0xa786eff8, 0x147c4e72, 0xb9807785, 0xafee48bb};
    EXPECT_EQ(sha256(message), expected_hash);
}

TEST(SHA256_Test, Short_message){
    std::string message = "Hello, world!";
    std::vector<uint32_t> expected_hash = {
       0x315f5bdb, 0x76d078c4, 0x3b8ac006, 0x4e4a0164, 0x612b1fce, 0x77c86934, 0x5bfc94c7, 0x5894edd3
    };
    EXPECT_EQ(sha256(message), expected_hash);
}

TEST(SHA256_Test, Long_message){
    std::string message(1000, 'a');
    std::vector<uint32_t> expected_hash = {
        0x41edece4, 0x2d63e8d9, 0xbf515a9b, 0xa6932e1c, 0x20cbc9f5, 0xa5d13464, 0x5adb5db1, 0xb9737ea3
    };
    EXPECT_EQ(sha256(message), expected_hash);
}

TEST(SHA256_Test, Special_characters){
    std::string message = "Special characters: !@#$%^&*()";
    std::vector<uint32_t> expected_hash = {
        0x3b2d26f6, 0xdb1f83a0, 0xb76c98d7, 0x7a3695fb, 0x8cc6127a, 0x6fa85b1f, 0xc8713321, 0xe4f8ce68
    };
    EXPECT_EQ(sha256(message), expected_hash);
}
