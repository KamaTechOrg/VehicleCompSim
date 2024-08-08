#include <gtest/gtest.h>
#include "rsaBigNum.h"
#include "BigNum.h"

TEST(RSA_BigNum_test, gcd_test)
{
    BigNum a("a");
    BigNum b("5");
    EXPECT_EQ(RSABigNum::gcd(b, a), b);
}

TEST(RSA_BigNum_test, gcd_test_with_512bit_1)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "9584fc2295db9bd232e18c6352d22d146e9c26693754f9045b28774886d3275eda82ba271bdfaad6eacc93a0ed37a05eb17a96e1e2b94c1cd5e329bfe95c8309";
    BigNum p(num1);
    // std::cout << "Prime p: " << p << std::endl;
    BigNum q(num2);
    // std::cout << "Prime q: " << q << std::endl;

    BigNum phi = (p - 1) * (q - 1);
    BigNum e("65537"); // Common choice for e

    EXPECT_EQ(RSABigNum::gcd(e, phi), 1);
}

TEST(RSA_BigNum_test, gcd_test_with_512bit_2)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "9584fc2295db9bd232e18c6352d22d146e9c26693754f9045b28774886d3275eda82ba271bdfaad6eacc93a0ed37a05eb17a96e1e2b94c1cd5e329bfe95c8309";
    BigNum p(num1);
    // std::cout << "Prime p: " << p << std::endl;
    BigNum q(num2);
    // std::cout << "Prime q: " << q << std::endl;

    EXPECT_EQ(RSABigNum::gcd(p, q), 1);
}

TEST(RSA_BigNum_test, gcd_test_with_512bit_3)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "1f78e4fd5b0ea80b57602a029559f04f9fb79a3eb120ec81867a29e1bed8472b0dffaaa4a5a8201fca845a0bff4762b63b73d763d12b98840937e1785142a12be";
    BigNum p(num1);
    // std::cout << "Prime p: " << p << std::endl;
    BigNum q(num2);
    // std::cout << "Prime q: " << q << std::endl;

    EXPECT_EQ(RSABigNum::gcd(q, p), p);
}

TEST(RSA_BigNum_test, modInverse_test_1)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "9584fc2295db9bd232e18c6352d22d146e9c26693754f9045b28774886d3275eda82ba271bdfaad6eacc93a0ed37a05eb17a96e1e2b94c1cd5e329bfe95c8309";
    BigNum p(num1);
    // std::cout << "Prime p: " << p << std::endl;
    BigNum q(num2);
    BigNum d("35c8d39b52bd4dc211d66eb037c9b3b1439328fb9ae690436e9c398dc4f32f7fc794272f90d2914882311c9bde7351c89988d96cb53d7b2c82d2fef7811744eb30038490a19edd25d122b5f264136abac6ee61343d8c4f994c4327af3010943b807aec072d226227a0a4b78700eb1cfa723e026edb8a937d5a3fa6bfa9600fe1");
    BigNum e("65537");
    BigNum phi = (p - 1) * (q - 1);
    // std::cout << "Prime q: " << q << std::endl;
    EXPECT_EQ(RSABigNum::modInverse(e, phi), d);
}

TEST(RSA_BigNum_test, modInverse_test_2)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("65537");
    BigNum phi = (p - 1) * (q - 1);

    EXPECT_EQ(RSABigNum::modInverse(e, phi), d);
}

// test RSA encryption and decryption for int

TEST(RSA_BigNum_test, encryption_and_decryption_tst_for_int)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("65537");
    BigNum message = 12345;

    BigNum encrypted = RSABigNum::encrypt(message, e, p * q);
    BigNum decrypted = RSABigNum::decrypt(encrypted, d, p * q);

    EXPECT_EQ(message, decrypted);
    EXPECT_NE(message, encrypted);
}

TEST(RSA_BigNum_test, encryption_and_decryption_tst_for_string)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("65537");
    std::string message = "12345";
    std::string encrypted = RSABigNum::encrypt(message, e, p * q);
    std::string decrypted = RSABigNum::decrypt(encrypted, d, p * q);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(message, encrypted);
}

TEST(RSA_BigNum_test, isPrime_test_with_prime_number)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    EXPECT_TRUE(RSABigNum::isPrime(p, 40));
    EXPECT_TRUE(RSABigNum::isPrime(q, 40));
}

TEST(RSA_BigNum_test, isPrime_test_with_non_prime_number)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    EXPECT_FALSE(RSABigNum::isPrime(p * 2, 40));
    EXPECT_FALSE(RSABigNum::isPrime(q * 2, 40));
}

TEST(RSA_BigNum_test, power_test1)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum a("12345");
    BigNum b("12345");
    BigNum c = RSABigNum::power(a, b, p * q);
    EXPECT_EQ(c, a);
}

TEST(RSA_BigNum_test, power_test2)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "1f78e4fd5b0ea80b57602a029559f04f9fb79a3eb120ec81867a29e1bed8472b0dffaaa4a5a8201fca845a0bff4762b63b73d763d12b98840937e1785142a12be";
    BigNum p(num1);
    BigNum q(num2);
    BigNum a("12345");
    BigNum b("12345");
    BigNum c = RSABigNum::power(p, q, a * b);
    EXPECT_EQ(c, a);
}

// TEST(RSA_BigNum_test, WithBigNum){
//     // std::cout << "Testing RSA with long int" << std::endl;
//     BigNum publicKey, privateKey, modulus;

//     RSABigNum::generate_keys(publicKey, privateKey, modulus, 256);

//     // test RSA encryption and decryption for int
//     BigNum message = 12345;
//     BigNum encrypted = RSABigNum::encrypt(message, publicKey, modulus);
//     BigNum decrypted = RSABigNum::decrypt(encrypted, privateKey, modulus);
//     EXPECT_EQ(message, decrypted);
//     EXPECT_NE(encrypted, message);

//     // if (message == decrypted && encrypted != message)
// 	// {
// 	// 	std::cout << "RSA encryption and decryption for int passed" << std::endl;
// 	// }
// 	// else
//     // {
// 	// 	std::cout << "RSA encryption and decryption for int failed" << std::endl;
//     // }
// }

// test RSA encryption and decryption for string
// TEST(RSA_BigNum_test , WithString){
//     BigNum publicKey, privateKey, modulus;
//     RSABigNum::generate_keys(publicKey, privateKey, modulus, 256);
//     std::string text = "Hello, RSABigNum!";
//     std::string encrypted_text = RSABigNum::encrypt_string(text, publicKey, modulus);
//     std::string decrypted_text = RSABigNum::decrypt_string(encrypted_text, privateKey, modulus);
//     EXPECT_EQ(text, decrypted_text);
//     EXPECT_NE(encrypted_text, text);

// 	// if (text == decrypted_text && encrypted_text != text)
// 	// {
// 	// 	std::cout << "RSABigNum encryption and decryption for string passed" << std::endl;
// 	// }
// 	// else
// 	// {
// 	// 	std::cout << "RSABigNum encryption and decryption for string failed" << std::endl;
//     //     std::cout << "publicKey: " << publicKey << std::endl;
//     //     std::cout << "privateKey: " << privateKey << std::endl;
//     //     std::cout << "modulus: " << modulus << std::endl;
// 	// }
// 	// std::cout << "Testing RSABigNum done" << std::endl;
// }
