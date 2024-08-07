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
#include "../BigNum/BigNum.h"
#include "../BigNum/BigNumWithMinus.h"


class RSABigNum
{
public:

    
    static void generate_keys(BigNum& publicKey, BigNum& privateKey, BigNum& modulus, int bits = 1024);
    static bool isPrime(const BigNum& n, int k);
    static BigNum power(const BigNum& x, const BigNum& y, const BigNum& p);


    static bool millerTest(const BigNum& d, const BigNum& n);
    static BigNum generateRandomNumber(int bits, int& count);
    static std::string generateRandomBits(size_t length);
    static BigNum generateLargePrime(int bits);
    static BigNum gcd(BigNum a, BigNum b);
    static BigNum modInverse(BigNum a, BigNum m);

    static BigNum encrypt(const BigNum& message, const BigNum& publicKey, const BigNum& modulus);

    static BigNum decrypt(const BigNum& message, const BigNum& praizvateKey, const BigNum& modulus);

    static std::string encrypt_string(const std::string& message, const BigNum& publicKey, const BigNum& modulus);

    static std::string decrypt_string(const std::string& encrypted_message, const BigNum& privateKey, const BigNum& modulus);
};




   