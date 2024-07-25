#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>

class RSA
{
public:
    static void generate_keys(unsigned long long int& publicKey, unsigned long long int& privateKey, unsigned long long int& modulus, int bits = 1024);
    static bool isPrime(unsigned long long n, int k);
    static unsigned long long power(unsigned long long x, unsigned long long y, unsigned long long p);


    static bool millerTest(unsigned long long d, unsigned long long n);
    static unsigned long long generateRandomNumber(int bits);
    static unsigned long long generateLargePrime(int bits);
    static unsigned long long gcd(unsigned long long a, unsigned long long b);
    static unsigned long long modInverse(unsigned long long a, unsigned long long m);

    static unsigned long long encrypt(unsigned long long int message, unsigned long long int publicKey, unsigned long long int modulus);

	static unsigned long long decrypt(unsigned long long int message, unsigned long long int privateKey, unsigned long long int modulus);

    static std::string encrypt_string(std::string message, unsigned long long int publicKey, unsigned long long int modulus);

	static std::string decrypt_string(std::string message, unsigned long long int privateKey, unsigned long long int modulus);
};




   