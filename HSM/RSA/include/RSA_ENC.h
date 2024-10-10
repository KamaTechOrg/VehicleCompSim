/**
 * @file RSA_ENC.h
 * @brief Header file for RSA encryption and decryption functions.
 *
 * This file contains the declaration of the RSA_ENC class, which provides
 * methods for encrypting and decrypting messages using RSA.
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

/**
 * @class RSA_ENC
 * @brief Class providing encryption and decryption using the RSA algorithm.
 */
class RSA_ENC
{
public:
    /**
     * @brief Encrypts a message using the RSA algorithm.
     * 
     * @param[in] message The message to be encrypted as a BigNum.
     * @param[in] publicKey The public key for encryption.
     * @param[in] modulus The modulus used in the RSA algorithm.
     * @return The encrypted message as a BigNum.
     */
    static BigNum encrypt(const BigNum &message, const BigNum &publicKey, const BigNum &modulus);

    /**
     * @brief Decrypts a message using the RSA algorithm.
     * 
     * @param[in] message The encrypted message as a BigNum.
     * @param[in] privateKey The private key for decryption.
     * @param[in] modulus The modulus used in the RSA algorithm.
     * @return The decrypted message as a BigNum.
     */
    static BigNum decrypt(const BigNum &message, const BigNum &privateKey, const BigNum &modulus);

    /**
     * @brief Encrypts a message using RSA with a byte vector input.
     * 
     * @param[in] message The message to be encrypted as a vector of bytes.
     * @param[in] publicKey The public key for encryption.
     * @param[in] modulus The modulus used in the RSA algorithm.
     * @return The encrypted message as a vector of bytes.
     */
    static std::vector<u_int8_t> encrypt(const std::vector<u_int8_t> &message, const BigNum &publicKey, const BigNum &modulus);

    /**
     * @brief Encrypts a message using RSA with a string input.
     * 
     * @param[in] message The message to be encrypted as a string.
     * @param[in] publicKey The public key for encryption.
     * @param[in] modulus The modulus used in the RSA algorithm.
     * @return The encrypted message as a string.
     */
    static std::string encrypt(const std::string &message, const BigNum &publicKey, const BigNum &modulus);

    /**
     * @brief Encrypts a message using a byte vector and a key.
     * 
     * @param[in] message The message to be encrypted as a vector of bytes.
     * @param[in] key The key used for encryption as a vector of bytes.
     * @return The encrypted message as a vector of bytes.
     */
    static std::vector<u_int8_t> encrypt(const std::vector<u_int8_t> &message, const std::vector<u_int8_t> &key);

    /**
     * @brief Decrypts a message using RSA with a byte vector input.
     * 
     * @param[in] encrypted_message The encrypted message as a vector of bytes.
     * @param[in] privateKey The private key for decryption.
     * @param[in] modulus The modulus used in the RSA algorithm.
     * @return The decrypted message as a vector of bytes.
     */
    static std::vector<u_int8_t> decrypt(const std::vector<u_int8_t> &encrypted_message, const BigNum &privateKey, const BigNum &modulus);

    /**
     * @brief Decrypts a message using RSA with a string input.
     * 
     * @param[in] encrypted_message The encrypted message as a string.
     * @param[in] privateKey The private key for decryption.
     * @param[in] modulus The modulus used in the RSA algorithm.
     * @return The decrypted message as a string.
     */
    static std::string decrypt(const std::string &encrypted_message, const BigNum &privateKey, const BigNum &modulus);

    /**
     * @brief Decrypts a message using a byte vector and a key.
     * 
     * @param[in] encrypted_message The encrypted message as a vector of bytes.
     * @param[in] key The key used for decryption as a vector of bytes.
     * @return The decrypted message as a vector of bytes.
     */
    static std::vector<u_int8_t> decrypt(const std::vector<u_int8_t> &encrypted_message, const std::vector<u_int8_t> &key);
};
