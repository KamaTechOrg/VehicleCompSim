#include <gtest/gtest.h>
#include "rsaBigNum.h"
#include "BigNum.h"

TEST(RSA_BigNum_test, WithBigNum){
    // std::cout << "Testing RSA with long int" << std::endl;
    BigNum publicKey, privateKey, modulus;

    RSABigNum::generate_keys(publicKey, privateKey, modulus, 256);

    // test RSA encryption and decryption for int
    BigNum message = 12345;
    BigNum encrypted = RSABigNum::encrypt(message, publicKey, modulus);
    BigNum decrypted = RSABigNum::decrypt(encrypted, privateKey, modulus);
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
TEST(RSA_BigNum_test , WithString){
    BigNum publicKey, privateKey, modulus;
    RSABigNum::generate_keys(publicKey, privateKey, modulus, 256);
    std::string text = "Hello, RSABigNum!";
    std::string encrypted_text = RSABigNum::encrypt_string(text, publicKey, modulus);
    std::string decrypted_text = RSABigNum::decrypt_string(encrypted_text, privateKey, modulus);
    EXPECT_EQ(text, decrypted_text);
    EXPECT_NE(encrypted_text, text);

	// if (text == decrypted_text && encrypted_text != text)
	// {
	// 	std::cout << "RSABigNum encryption and decryption for string passed" << std::endl;
	// }
	// else
	// {
	// 	std::cout << "RSABigNum encryption and decryption for string failed" << std::endl;
    //     std::cout << "publicKey: " << publicKey << std::endl;
    //     std::cout << "privateKey: " << privateKey << std::endl;
    //     std::cout << "modulus: " << modulus << std::endl;
	// }
	// std::cout << "Testing RSABigNum done" << std::endl;
}
