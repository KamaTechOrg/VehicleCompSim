#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include "../BigNum/BigNum.h"

class RSA
{
public:

    
    static void generate_keys(BigNum& publicKey, BigNum& privateKey, BigNum& modulus, int bits = 1024);
    static bool isPrime(BigNum n, int k);
    static BigNum power(BigNum x, BigNum y, BigNum p);


    static bool millerTest(BigNum d, BigNum n);
    static BigNum generateRandomNumber(int bits);
    static BigNum generateLargePrime(int bits);
    static BigNum gcd(BigNum a, BigNum b);
    static BigNum modInverse(BigNum a, BigNum m);

    static BigNum encrypt(BigNum message, BigNum publicKey, BigNum modulus);

    static BigNum decrypt(BigNum message, BigNum privateKey, BigNum modulus);

    static std::string encrypt_string(std::string message, BigNum publicKey, BigNum modulus);

    static std::string decrypt_string(std::string message, BigNum privateKey, BigNum modulus);
};




   