
#include <cassert>
#include "SHA-256.h"
#include <iostream>

void runTests() {
    std::string message1 = "";
    std::vector<uint32_t> expected_hash1 = {
        0xe3b0c442, 0x98fc1c14, 0x9afbf4c8, 0x996fb924, 0x27ae41e4, 0x649b934c, 0xa495991b, 0x7852b855
    };
    assert(compareHashes(sha256(message1), expected_hash1));
    std::cout << "Test 1 passed: Empty message" << std::endl;


    std::string message2 = "a";
    std::vector<uint32_t> expected_hash2 = {
       0xca978112, 0xca1bbdca, 0xfac231b3, 0x9a23dc4d, 0xa786eff8, 0x147c4e72, 0xb9807785, 0xafee48bb
    };
    assert(compareHashes(sha256(message2), expected_hash2));
    std::cout << "Test 2 passed: Single character" << std::endl;

    std::string message3 = "Hello, world!";
    std::vector<uint32_t> expected_hash3 = {
        0x315f5bdb, 0x76d078c4, 0x3b8ac006, 0x4e4a0164, 0x612b1fce, 0x77c86934, 0x5bfc94c7, 0x5894edd3
    };
    assert(compareHashes(sha256(message3), expected_hash3));
    std::cout << "Test 3 passed: Short message" << std::endl;

    std::string message4(1000, 'a'); 
    std::vector<uint32_t> expected_hash4 = {
        0x41edece4, 0x2d63e8d9, 0xbf515a9b, 0xa6932e1c, 0x20cbc9f5, 0xa5d13464, 0x5adb5db1, 0xb9737ea3
    };
    assert(compareHashes(sha256(message4), expected_hash4));
    std::cout << "Test 4 passed: Long message" << std::endl;

    
    std::string message5 = "Special characters: !@#$%^&*()";
    std::vector<uint32_t> expected_hash5 = {
        0x3b2d26f6, 0xdb1f83a0, 0xb76c98d7, 0x7a3695fb, 0x8cc6127a, 0x6fa85b1f, 0xc8713321, 0xe4f8ce68
    };
    assert(compareHashes(sha256(message5), expected_hash5));
    std::cout << "Test 5 passed: Special characters" << std::endl;
}

int main() {

	runTests();
    return 0;
}
