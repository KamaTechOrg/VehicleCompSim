/**
 * @file BigNumWithMinus.h
 * @brief A header file defining the BigNumWithMinus class, which extends BigNum to support negative numbers.
 */

#pragma once
#include "BigNum.h"
#include <bits/stdc++.h>

/**
 * @class BigNumWithMinus
 * @brief A class that extends BigNum to represent and handle negative numbers.
 */
class BigNumWithMinus : public BigNum
{
    // BigNumWithMinus inherits from BigNum. BigNumWithMinus is fixed to max size.
public:
    /**
     * @brief Default constructor that initializes a BigNumWithMinus object with a fixed maximum size.
     */
    BigNumWithMinus() : BigNum(MAX_SIZE * UINT_T_SIZE) {}

    /**
     * @brief Constructor to initialize a BigNumWithMinus from a string.
     * If the string starts with a '-' character, the number is treated as negative.
     * @param num The string representing the number.
     */
    BigNumWithMinus(std::string num);

    /**
     * @brief Constructor to initialize a BigNumWithMinus from a vector of bytes.
     * If the vector starts with a '-' character, the number is treated as negative.
     * @param num The vector of 8-bit unsigned integers representing the number as a ascii string.
     */
    BigNumWithMinus(std::vector<u_int8_t> num);

    /**
     * @brief Copy constructor to initialize a BigNumWithMinus from another BigNum.
     * @param other The BigNum object to copy.
     */
    BigNumWithMinus(const BigNum &other) : BigNum(MAX_SIZE * UINT_T_SIZE)
    {
        for (int i = 0; i < other.size; ++i)
        {
            this->data[i] = other.data[i];
        }
    }

    /**
     * @brief Copy constructor to initialize a BigNumWithMinus from another BigNumWithMinus.
     * @param other The BigNumWithMinus object to copy.
     */
    BigNumWithMinus(const BigNumWithMinus &other) : BigNum(MAX_SIZE * UINT_T_SIZE)
    {
        for (int i = 0; i < other.size; ++i)
        {
            this->data[i] = other.data[i];
        }
    }

    /**
     * @brief Constructor to initialize a BigNumWithMinus with a specific bit size.
     * @param bit_size The size of the number in bits.
     */
    BigNumWithMinus(int bit_size) : BigNum(MAX_SIZE * UINT_T_SIZE) {}

    /**
     * @brief Constructor to initialize a BigNumWithMinus from a 32-bit unsigned integer.
     * @param num A 32-bit unsigned integer to initialize the BigNumWithMinus.
     * @param bit_size The size of the number in bits.
     */

    BigNumWithMinus(uint32_t num, int bit_size) : BigNum(num, MAX_SIZE * UINT_T_SIZE) {}

    /**
     * @brief Sets the BigNumWithMinus to represent a negative number.
     */
    void setToMinus();

    /**
     * @brief Sets the BigNumWithMinus to represent a positive number.
     */
    void setToPlus();

    // Comparison Operators

    /**
     * @brief Less-than operator for comparing two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return True if this BigNumWithMinus is less than the other, otherwise false.
     */
    bool operator<(const BigNumWithMinus &other) const;

    /**
     * @brief Less-than operator for comparing a BigNumWithMinus and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNumWithMinus is less than the integer, otherwise false.
     */
    bool operator<(uint32_t num) const;

    /**
     * @brief Greater-than operator for comparing two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return True if this BigNumWithMinus is greater than the other, otherwise false.
     */
    bool operator>(const BigNumWithMinus &other) const;

    /**
     * @brief Greater-than operator for comparing a BigNumWithMinus and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNumWithMinus is greater than the integer, otherwise false.
     */
    bool operator>(uint32_t num) const;

    /**
     * @brief Less-than-or-equal-to operator for comparing two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return True if this BigNumWithMinus is less than or equal to the other, otherwise false.
     */
    bool operator<=(const BigNumWithMinus &other) const;

    /**
     * @brief Less-than-or-equal-to operator for comparing a BigNumWithMinus and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNumWithMinus is less than or equal to the integer, otherwise false.
     */
    bool operator<=(uint32_t num) const;

    /**
     * @brief Greater-than-or-equal-to operator for comparing two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return True if this BigNumWithMinus is greater than or equal to the other, otherwise false.
     */
    bool operator>=(const BigNumWithMinus &other) const;

    /**
     * @brief Greater-than-or-equal-to operator for comparing a BigNumWithMinus and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNumWithMinus is greater than or equal to the integer, otherwise false.
     */
    bool operator>=(uint32_t num) const;

    // Arithmetic Operators

    /**
     * @brief Subtraction operator for subtracting two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return A new BigNumWithMinus representing the difference.
     */
    BigNumWithMinus operator-(const BigNumWithMinus &other) const;

    /**
     * @brief Subtraction and assignment operator for subtracting another BigNumWithMinus from this BigNumWithMinus.
     * @param other The other BigNumWithMinus object.
     * @return A reference to the current BigNumWithMinus object after subtraction.
     */
    BigNumWithMinus &operator-=(const BigNumWithMinus &other);

    /**
     * @brief Multiplication operator for multiplying two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return A new BigNumWithMinus representing the product.
     */
    BigNumWithMinus operator*(const BigNumWithMinus &other) const;

    /**
     * @brief Multiplication and assignment operator for multiplying this BigNumWithMinus by another BigNumWithMinus.
     * @param other The other BigNumWithMinus object.
     * @return A reference to the current BigNumWithMinus object after multiplication.
     */
    BigNumWithMinus &operator*=(const BigNumWithMinus &other);

    /**
     * @brief Division operator for dividing two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return A new BigNumWithMinus representing the quotient.
     */
    BigNumWithMinus operator/(const BigNumWithMinus &other) const;

    /**
     * @brief Division operator for dividing a BigNumWithMinus by a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A new BigNumWithMinus representing the quotient.
     */
    BigNumWithMinus operator/(uint32_t num) const;

    /**
     * @brief Division and assignment operator for dividing this BigNumWithMinus by another BigNumWithMinus.
     * @param other The other BigNumWithMinus object.
     * @return A reference to the current BigNumWithMinus object after division.
     */
    BigNumWithMinus &operator/=(const BigNumWithMinus &other);

    /**
     * @brief Modulus operator for finding the remainder of division between two BigNumWithMinus objects.
     * @param other The other BigNumWithMinus object.
     * @return A new BigNumWithMinus representing the remainder.
     */
    BigNumWithMinus operator%(const BigNumWithMinus &other) const;

    /**
     * @brief Modulus operator for finding the remainder of division between a BigNumWithMinus and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A new BigNumWithMinus representing the remainder.
     */
    BigNumWithMinus operator%(uint32_t num) const;

    // Utility Functions

    /**
     * @brief Checks if the BigNumWithMinus is a negative number.
     * @return True if the number is negative, otherwise false.
     */
    bool isMinus() const;

    /**
     * @brief Prints the BigNumWithMinus in a human-readable format (for testing purposes).
     */
    void print() const;

    /**
     * @brief Converts the BigNumWithMinus to a string representation.
     * @return A string representing the number.
     */
    std::string toString() const;

    /**
     * @brief Converts the BigNumWithMinus to a vector of 8-bit unsigned integers.
     * @return A vector of bytes representing the number.
     */
    std::vector<u_int8_t> toVectorChar() const;
};
