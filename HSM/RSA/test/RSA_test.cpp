#include <gtest/gtest.h>
#include "RSA_ENC.h"
#include "RSA_KEY.h"
#include "../../BigNum/include/BigNum.h"


TEST(RSA_BigNum_test, gcd_test)
{
    BigNum a("a");
    BigNum b("5");
    EXPECT_EQ(RSA_KEY::gcd(b, a), b);
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
    BigNum e("10001"); // Common choice for e

    EXPECT_EQ(RSA_KEY::gcd(e, phi), 1);
}

TEST(RSA_BigNum_test, gcd_test_with_512bit_2)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "9584fc2295db9bd232e18c6352d22d146e9c26693754f9045b28774886d3275eda82ba271bdfaad6eacc93a0ed37a05eb17a96e1e2b94c1cd5e329bfe95c8309";
    BigNum p(num1);
    // std::cout << "Prime p: " << p << std::endl;
    BigNum q(num2);
    // std::cout << "Prime q: " << q << std::endl;

    EXPECT_EQ(RSA_KEY::gcd(p, q), 1);
}

TEST(RSA_BigNum_test, gcd_test_with_512bit_3)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "1f78e4fd5b0ea80b57602a029559f04f9fb79a3eb120ec81867a29e1bed8472b0dffaaa4a5a8201fca845a0bff4762b63b73d763d12b98840937e1785142a12be";
    BigNum p(num1);
    // std::cout << "Prime p: " << p << std::endl;
    BigNum q(num2);
    // std::cout << "Prime q: " << q << std::endl;

    EXPECT_EQ(RSA_KEY::gcd(q, p), p);
}

TEST(RSA_BigNum_test, modInverse_test_1)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "9584fc2295db9bd232e18c6352d22d146e9c26693754f9045b28774886d3275eda82ba271bdfaad6eacc93a0ed37a05eb17a96e1e2b94c1cd5e329bfe95c8309";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("35c8d39b52bd4dc211d66eb037c9b3b1439328fb9ae690436e9c398dc4f32f7fc794272f90d2914882311c9bde7351c89988d96cb53d7b2c82d2fef7811744eb30038490a19edd25d122b5f264136abac6ee61343d8c4f994c4327af3010943b807aec072d226227a0a4b78700eb1cfa723e026edb8a937d5a3fa6bfa9600fe1");
    BigNum e("10001");
    BigNum phi = (p - 1) * (q - 1);
    EXPECT_EQ(RSA_KEY::modInverse(e, phi), d);
}

TEST(RSA_BigNum_test, modInverse_test_2)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("10001");
    BigNum phi = (p - 1) * (q - 1);

    EXPECT_EQ(RSA_KEY::modInverse(e, phi), d);
}

TEST(RSA_BigNum_test, modInverse_test_3)
{
    std::string num1 = "33c46f1f3e1f281314d7978ccbbf855d";
    std::string num2 = "b6a1080240cb915cc873906d458492bf";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("15e93dd7ddb120dbe554a78aad68214341bbe428efaf6ba5e2c5d9bc73928edd");
    BigNum e("f212ec50962209fb413f34d63b184e4d");
    BigNum phi = (p - 1) * (q - 1);
    EXPECT_EQ(RSA_KEY::modInverse(e, phi), d);
}

// test RSA encryption and decryption for int

TEST(RSA_BigNum_test, encryption_and_decryption_tst_for_int)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("10001");
    BigNum message("12345");

    BigNum encrypted = RSA_KEY::power(message, e, p * q);
    BigNum decrypted = RSA_ENC::decrypt(encrypted, d, p * q);
    BigNum expected("1ceee7cc810c265");

    EXPECT_EQ(message, decrypted);
    EXPECT_EQ(expected, encrypted);
}

TEST(RSA_BigNum_test, encryption_and_decryption_tst_for_int_2)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("10001");
    BigNum message(825373492,64);

    BigNum encrypted = RSA_ENC::encrypt(message, e, p * q);
    std::string encrypted1 = encrypted.toString();
    BigNum decrypted = RSA_ENC::decrypt(encrypted, d, p * q);

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
    BigNum e("10001");
    std::string message = "1234";
    // uint64_t encryptedInLong = 825373492;
    // BigNum encrypted1 = RSA_ENC::encrypt(BigNum(encryptedInLong,64), e, p * q);
    std::vector<u_char> encrypted = RSA_ENC::encrypt(std::vector<u_char>(message.begin(), message.end()), e, p * q);
    std::vector<u_char> decrypted = RSA_ENC::decrypt(encrypted, d, p * q);
    EXPECT_EQ("e70802ea7e14079P", encrypted);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(message, encrypted);
}

TEST(RSA_BigNum_test, encryption_and_decryption_tst_for_string_2)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("10001");
    std::string text = "Hello";
    std::vector<u_char> encrypted_text = RSA_ENC::encrypt(std::vector<u_char>(text.begin(), text.end()), e, p * q);
    std::vector<u_char> decrypted_text = RSA_ENC::decrypt(encrypted_text, d, p * q);
    EXPECT_EQ(text, decrypted_text);
    EXPECT_NE(text, encrypted_text); 
}

TEST(RSA_BigNum_test, encryption_and_decryption_tst_for_string_3)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("10001");
    std::string message = "12345";
    std::vector<u_char> encrypted = RSA_ENC::encrypt(std::vector<u_char>(message.begin(), message.end()), e, p * q);
    std::vector<u_char> decrypted = RSA_ENC::decrypt(encrypted, d, p * q);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(message, encrypted);
}

TEST(RSA_BigNum_test, encryption_and_decryption_tst_for_string_4)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum d("13dcfa5db7bcc201");
    BigNum e("10001");
    std::string message = "Hello World, this is my secret message!";
    std::vector<u_char> encrypted = RSA_ENC::encrypt(std::vector<u_char>(message.begin(), message.end()), e, p * q);
    std::vector<u_char> decrypted = RSA_ENC::decrypt(encrypted, d, p * q);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(message, encrypted);
}

TEST(RSA_BigNum_test, isPrime_test_with_prime_number)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    EXPECT_TRUE(RSA_KEY::isPrime(p, 40));
    EXPECT_TRUE(RSA_KEY::isPrime(q, 40));
}

TEST(RSA_BigNum_test, isPrime_test_with_non_prime_number)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    EXPECT_FALSE(RSA_KEY::isPrime(p * 2, 40));
    EXPECT_FALSE(RSA_KEY::isPrime(q * 2, 40));
}

TEST(RSA_BigNum_test, power_test1)
{
    std::string num1 = "3c07baa3";
    std::string num2 = "614557f1";
    BigNum p(num1);
    BigNum q(num2);
    BigNum a("12345");
    BigNum b("12345");
    BigNum result("1454aef10701f520");
    BigNum c = RSA_KEY::power(a, b, p * q);
    EXPECT_EQ(c, result);
}

TEST(RSA_BigNum_test, power_test2)
{
    std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
    std::string num2 = "1f78e4fd5b0ea80b57602a029559f04f9fb79a3eb120ec81867a29e1bed8472b0dffaaa4a5a8201fca845a0bff4762b63b73d763d12b98840937e1785142a12be";
    BigNum p(num1);
    BigNum q(num2);
    BigNum a("12345");
    BigNum result("b4a9");
    BigNum c = RSA_KEY::power(p, q, a);
    EXPECT_EQ(c, result);
}

TEST(RSA_BigNum_test, power_test3)
{
    BigNum x("2");
    BigNum y("3");
    BigNum mod("5");
    BigNum exp("3");
    BigNum c = RSA_KEY::power(x, y, mod);
    EXPECT_EQ(c, exp);
}

TEST(RSA_BigNum_test, power_test4)
{
    BigNum x("2");
    BigNum y("3");
    BigNum mod("50");
    BigNum exp("8");
    BigNum c = RSA_KEY::power(x, y, mod);
    EXPECT_EQ(c, exp);
}

TEST(RSA_BigNum_test, WithBigNum_1)
{
    // std::cout << "Testing RSA with long int" << std::endl;
    BigNum publicKey, privateKey, modulus;

    RSA_KEY::generate_keys(publicKey, privateKey, modulus, 256);

    // test RSA encryption and decryption for int
    BigNum message("12345");
    

    BigNum encrypted = RSA_ENC::encrypt(message, publicKey, modulus);
    BigNum decrypted = RSA_ENC::decrypt(encrypted, privateKey, modulus);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(encrypted, message);
}

TEST(RSA_BigNum_test, WithBigNum_2)
{
    // std::cout << "Testing RSA with long int" << std::endl;
    BigNum publicKey, privateKey, modulus;

    RSA_KEY::generate_keys(publicKey, privateKey, modulus, 512);

    // test RSA encryption and decryption for int
    BigNum message("12345768910");
    BigNum encrypted = RSA_ENC::encrypt(message, publicKey, modulus);
    BigNum decrypted = RSA_ENC::decrypt(encrypted, privateKey, modulus);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(encrypted, message);
}

TEST(RSA_BigNum_test, WithBigNum_3)
{
    // std::cout << "Testing RSA with long int" << std::endl;
    BigNum publicKey, privateKey, modulus;

    RSA_KEY::generate_keys(publicKey, privateKey, modulus, 1024);

    // test RSA encryption and decryption for int
    BigNum message("12345768910");
    BigNum encrypted = RSA_ENC::encrypt(message, publicKey, modulus);
    BigNum decrypted = RSA_ENC::decrypt(encrypted, privateKey, modulus);
    EXPECT_EQ(message, decrypted);
    EXPECT_NE(encrypted, message);
}
// // test RSA encryption and decryption for string
TEST(RSA_BigNum_test, WithString)
{
    BigNum publicKey, privateKey, modulus;
    RSA_KEY::generate_keys(publicKey, privateKey, modulus, 256);
    std::string text = {"Hello, RSA!"};
    std::vector<u_char> encrypted_text = RSA_ENC::encrypt(std::vector<u_char>(text.begin(), text.end()), publicKey, modulus);
    std::vector<u_char> decrypted_text = RSA_ENC::decrypt(encrypted_text, privateKey, modulus);
    EXPECT_EQ(text, decrypted_text);
    EXPECT_NE(encrypted_text, text);
}
