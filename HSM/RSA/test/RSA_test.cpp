#include <gtest/gtest.h>
#include "RSA.h"

TEST(RSA_Test, WithLongInt){
    // std::cout << "Testing RSA with long int" << std::endl;
    unsigned long long int publicKey, privateKey, modulus;

    RSA::generate_keys(publicKey, privateKey, modulus, 32);

    // test RSA encryption and decryption for int
    unsigned long long int message = 12345;
    unsigned long long int encrypted = RSA::encrypt(message, publicKey, modulus);
    unsigned long long int decrypted = RSA::decrypt(encrypted, privateKey, modulus);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(encrypted, message);

    // if (message == decrypted && encrypted != message)
	// {
	// 	std::cout << "RSA encryption and decryption for int passed" << std::endl;
	// }
	// else
    // {
	// 	std::cout << "RSA encryption and decryption for int failed" << std::endl;
    // }
}



// test RSA encryption and decryption for string
TEST(RSA_Test, WithString){
    unsigned long long int publicKey, privateKey, modulus;
    RSA::generate_keys(publicKey, privateKey, modulus, 32);
    std::string text = "Hello, RSA!";
    std::string encrypted_text = RSA::encrypt_string(text, publicKey, modulus);
    std::string decrypted_text = RSA::decrypt_string(encrypted_text, privateKey, modulus);
    EXPECT_EQ(text, decrypted_text);
    EXPECT_NE(encrypted_text, text);

	// if (text == decrypted_text && encrypted_text != text)
	// {
	// 	std::cout << "RSA encryption and decryption for string passed" << std::endl;
	// }
	// else
	// {
	// 	std::cout << "RSA encryption and decryption for string failed" << std::endl;
    //     std::cout << "publicKey: " << publicKey << std::endl;
    //     std::cout << "privateKey: " << privateKey << std::endl;
    //     std::cout << "modulus: " << modulus << std::endl;
	// }
	// std::cout << "Testing RSA done" << std::endl;
}
