#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include "../../BigNum/include/BigNum.h"
#include "../../BigNum/include/BigNumWithMinus.h"

class RSA
{
public:
    static std::pair<std::vector<u_char>, std::vector<u_char>> generateKeys(const std::vector<u_char>& type);

    static void generate_keys(BigNum &publicKey, BigNum &privateKey, BigNum &modulus, int bits = 1024);
    static bool isPrime(const BigNum &n, int k);
    static BigNum power(const BigNum &x, const BigNum &y, const BigNum &p);

    static bool millerTest(const BigNum &d, const BigNum &n);
    static BigNum generateRandomNumber(int bits);
    static std::vector<u_char> generateRandomBits(size_t length);
    static BigNum generateLargePrime(int bits);
    static BigNum gcd(BigNum a, BigNum b);
    static BigNum modInverse(BigNum a, BigNum m);

    static BigNum encrypt(const BigNum &message, const BigNum &publicKey, const BigNum &modulus);

    static BigNum decrypt(const BigNum &message, const BigNum &privateKey, const BigNum &modulus);

    static std::vector<u_char> encrypt(const std::vector<u_char> &message, const BigNum &publicKey, const BigNum &modulus);
    static std::vector<u_char> encrypt(const std::vector<u_char> &message, const std::vector<u_char>& key);

    static std::vector<u_char> decrypt(const std::vector<u_char> &encrypted_message, const BigNum &privateKey, const BigNum &modulus);

    static std::vector<u_char> decrypt(const std::vector<u_char> &encrypted_message, const std::vector<u_char>& key);
};































// #pragma once
// #include <iostream>
// #include <random>
// #include <ctime>
// #include <cmath>
// #include <string>
// #include <sstream>

// class RSA
// {
// public:
//     static void generate_keys(unsigned long long int& publicKey, unsigned long long int& privateKey, unsigned long long int& modulus, int bits = 1024);
//     static bool isPrime(unsigned long long n, int k);
//     static unsigned long long power(unsigned long long x, unsigned long long y, unsigned long long p);


//     static bool millerTest(unsigned long long d, unsigned long long n);
//     static unsigned long long generateRandomNumber(int bits);
//     static unsigned long long generateLargePrime(int bits);
//     static unsigned long long gcd(unsigned long long a, unsigned long long b);
//     static unsigned long long modInverse(unsigned long long a, unsigned long long m);

//     static unsigned long long encrypt(unsigned long long int message, unsigned long long int publicKey, unsigned long long int modulus);

// 	static unsigned long long decrypt(unsigned long long int message, unsigned long long int privateKey, unsigned long long int modulus);

//     static std::string encrypt_string(std::string message, unsigned long long int publicKey, unsigned long long int modulus);

// 	static std::string decrypt_string(std::string message, unsigned long long int privateKey, unsigned long long int modulus);
// };




   