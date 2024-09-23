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
#include "HSM_status_enum.cpp"


class RSA_KEY
{
public:
    HSM_STATUS RSA_KEY::generateKeys(std::vector<u_char> &public_key, std::vector<u_char> &private_key, int bits = 512);

    static void generate_keys(BigNum &publicKey, BigNum &privateKey, BigNum &modulus, int bits = 512);
    static bool isPrime(const BigNum &n, int k);
    static BigNum power(const BigNum &x, const BigNum &y, const BigNum &p);

    static bool millerTest(const BigNum &d, const BigNum &n);
    static BigNum generateRandomNumber(int bits);
    static std::vector<u_char> generateRandomBits(size_t length);
    static BigNum generateLargePrime(int bits);
    static BigNum gcd(BigNum a, BigNum b);
    static BigNum modInverse(BigNum a, BigNum m);
};