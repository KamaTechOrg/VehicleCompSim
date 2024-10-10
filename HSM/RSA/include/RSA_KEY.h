/**
 * @file RSA_KEY.h
 * @brief Header file for RSA key generation and related functions.
 *
 * This file contains the declaration of the RSA_KEY class, which provides 
 * functions for generating RSA keys, testing primality, and other cryptographic operations.
 */

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
#include "HSMEnums.hpp"

/**
 * @class RSA_KEY
 * @brief Class providing RSA key generation and related cryptographic operations.
 */
class RSA_KEY
{
public:
    /**
     * @brief Generates RSA public and private keys.
     * 
     * @param[out] public_key The generated public key as a vector of bytes.
     * @param[out] private_key The generated private key as a vector of bytes.
     * @param[in] bits The size of the keys in bits. Default is 512.
     * @return HSMns::HSM_STATUS indicating the success or failure of the key generation.
     */
    static HSMns::HSM_STATUS generateKeys(std::vector<u_int8_t> &public_key, std::vector<u_int8_t> &private_key, int bits = 512);

    /**
     * @brief Generates RSA public and private keys using BigNum.
     * 
     * @param[out] publicKey The generated public key as a BigNum.
     * @param[out] privateKey The generated private key as a BigNum.
     * @param[out] modulus The modulus used in the RSA algorithm.
     * @param[in] bits The size of the keys in bits. Default is 512.
     */
    static void generate_keys(BigNum &publicKey, BigNum &privateKey, BigNum &modulus, int bits = 512);

    /**
     * @brief Tests if a given number is prime using the Miller-Rabin primality test.
     * 
     * @param[in] n The number to test for primality.
     * @param[in] k The number of iterations for the test. Higher values of k increase accuracy.
     * @return True if the number is prime, false otherwise.
     */
    static bool isPrime(const BigNum &n, int k);

    /**
     * @brief Performs modular exponentiation.
     * 
     * @param[in] x The base.
     * @param[in] y The exponent.
     * @param[in] p The modulus.
     * @return The result of (x^y) % p.
     */
    static BigNum power(const BigNum &x, const BigNum &y, const BigNum &p);

    /**
     * @brief Performs a single iteration of the Miller-Rabin primality test.
     * 
     * @param[in] d A value computed as n-1 divided by a power of 2.
     * @param[in] n The number to test for primality.
     * @return True if n passes this iteration of the test, false otherwise.
     */
    static bool millerTest(const BigNum &d, const BigNum &n);

    /**
     * @brief Generates a random BigNum with the specified number of bits.
     * 
     * @param[in] bits The number of bits for the random number.
     * @return A random BigNum of the specified size.
     */
    static BigNum generateRandomNumber(int bits);

    /**
     * @brief Generates a random sequence of bits.
     * 
     * @param[in] length The length of the sequence in bytes.
     * @return A vector of random bytes.
     */
    static std::vector<u_int8_t> generateRandomBits(size_t length);

    /**
     * @brief Generates a large prime number with the specified number of bits.
     * 
     * @param[in] bits The size of the prime number in bits.
     * @return A prime BigNum of the specified size.
     */
    static BigNum generateLargePrime(int bits);

    /**
     * @brief Computes the greatest common divisor (GCD) of two BigNums.
     * 
     * @param[in] a The first number.
     * @param[in] b The second number.
     * @return The GCD of a and b.
     */
    static BigNum gcd(BigNum a, BigNum b);

    /**
     * @brief Computes the modular inverse of a number.
     * 
     * @param[in] a The number to find the modular inverse of.
     * @param[in] m The modulus.
     * @return The modular inverse of a modulo m.
     */
    static BigNum modInverse(BigNum a, BigNum m);
};
