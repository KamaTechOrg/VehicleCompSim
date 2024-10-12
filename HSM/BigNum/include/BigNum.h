/**
 * @file BigNum.h
 * @brief A header file defining the BigNum class for handling large integers up to 2048 bits.
 */

#pragma once
#include <iostream>
#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdint>

/**
 * @class BigNum
 * @brief A class for representing and performing operations on large numbers (up to 2048 bits).
 */
class BigNum {
public:
    static const int MAX_SIZE = 64;  ///< Maximum size of the number in 32-bit units.
    static const int UINT_T_SIZE = 32;  ///< Size of each unit in bits (32 bits).

    int size;  ///< Current size of the BigNum.
    std::vector<uint32_t> data;  ///< Internal storage for the number, stored as an array of 32-bit units.

    /**
     * @brief Default constructor. Initializes the BigNum to a size of MAX_SIZE with all elements set to zero.
     */
    BigNum() : size(MAX_SIZE) {
		data.resize(size, 0);
    }

    /**
     * @brief Constructor to initialize a BigNum with a specific bit size.
     * @param bit_size The size of the number in bits.
     */
    BigNum(int bit_size);

    /**
     * @brief Constructor to initialize a BigNum from a 64-bit unsigned integer.
     * @param num A 64-bit unsigned integer to initialize the BigNum.
     * @param bit_size The size of the number in bits.
     */
    BigNum(uint64_t num, int bit_size);

    /**
     * @brief Constructor to initialize a BigNum from a vector of bytes.
     * @param num A vector of 8-bit unsigned integers representing the number as a ascii string.
     */
    BigNum(std::vector<u_int8_t> num);

    /**
     * @brief Constructor to initialize a BigNum from a string representation of a number.
     * @param num A string representing the number.
     */
    BigNum(std::string num);

    /**
     * @brief Prints the BigNum in a human-readable format (for testing purposes).
     */
    void print() const;

    /**
     * @brief Converts the BigNum to a string representation.
     * @return A string representing the number.
     */
    std::string toString() const;

    /**
     * @brief Converts the BigNum to a vector of 8-bit unsigned integers.
     * @return A vector of bytes representing the number.
     */
    std::vector<u_int8_t> toVectorChar() const;

    // Arithmetic Operators

    /**
     * @brief Addition operator for adding two BigNum objects.
     * @param other The other BigNum object.
     * @return A new BigNum representing the sum.
     */
    BigNum operator+(const BigNum& other) const;

    /**
     * @brief Addition operator for adding a BigNum and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A new BigNum representing the sum.
     */
    BigNum operator+(uint32_t num) const;

    /**
     * @brief Addition and assignment operator for adding another BigNum to this BigNum.
     * @param other The other BigNum object.
     * @return A reference to the current BigNum object after addition.
     */
    BigNum operator+=(const BigNum& other);

    /**
     * @brief Addition and assignment operator for adding a 32-bit unsigned integer to this BigNum.
     * @param num The 32-bit unsigned integer.
     * @return A reference to the current BigNum object after addition.
     */
    BigNum operator+=(uint32_t num);

    // Multiplication Operators

    /**
     * @brief Multiplication operator for multiplying two BigNum objects.
     * @param other The other BigNum object.
     * @return A new BigNum representing the product.
     */
    BigNum operator*(const BigNum& other) const;

    /**
     * @brief Multiplication operator for multiplying a BigNum and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A new BigNum representing the product.
     */
    BigNum operator*(uint32_t num) const;

    /**
     * @brief Multiplication and assignment operator for multiplying this BigNum with another BigNum.
     * @param other The other BigNum object.
     * @return A reference to the current BigNum object after multiplication.
     */
    BigNum operator*=(const BigNum& other);

    /**
     * @brief Multiplication and assignment operator for multiplying this BigNum with a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A reference to the current BigNum object after multiplication.
     */
    BigNum operator*=(uint32_t num);

    // Subtraction Operators

    /**
     * @brief Subtraction operator for subtracting another BigNum from this BigNum.
     * @param other The other BigNum object.
     * @return A new BigNum representing the difference.
     */
    BigNum operator-(const BigNum& other) const;

    /**
     * @brief Subtraction operator for subtracting a 32-bit unsigned integer from this BigNum.
     * @param num The 32-bit unsigned integer.
     * @return A new BigNum representing the difference.
     */
    BigNum operator-(uint32_t num) const;

    /**
     * @brief Subtraction and assignment operator for subtracting another BigNum from this BigNum.
     * @param other The other BigNum object.
     * @return A reference to the current BigNum object after subtraction.
     */
    BigNum operator-=(const BigNum& other);

    /**
     * @brief Subtraction and assignment operator for subtracting a 32-bit unsigned integer from this BigNum.
     * @param num The 32-bit unsigned integer.
     * @return A reference to the current BigNum object after subtraction.
     */
    BigNum operator-=(uint32_t num);

    // Modulus, Division, and Shift Operators

    /**
     * @brief Modulus operator for finding the remainder of division between two BigNum objects.
     * @param other The other BigNum object.
     * @return A new BigNum representing the remainder.
     */
    BigNum operator%(const BigNum& other) const;

    /**
     * @brief Modulus operator for finding the remainder of division between a BigNum and a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A new BigNum representing the remainder.
     */
    BigNum operator%(uint32_t num) const;

    /**
     * @brief Division operator for dividing two BigNum objects.
     * @param other The other BigNum object.
     * @return A new BigNum representing the quotient.
     */
    BigNum operator/(const BigNum& other) const;

    /**
     * @brief Division operator for dividing a BigNum by a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A new BigNum representing the quotient.
     */
    BigNum operator/(uint32_t num) const;

    /**
     * @brief Division and assignment operator for dividing this BigNum by another BigNum.
     * @param other The other BigNum object.
     * @return A reference to the current BigNum object after division.
     */
    BigNum operator/=(const BigNum& other);

    /**
     * @brief Division and assignment operator for dividing this BigNum by a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return A reference to the current BigNum object after division.
     */
    BigNum operator/=(uint32_t num);

    // Bitwise Shift Operators

    /**
     * @brief Left shift operator for shifting the BigNum by a certain number of bits to the left.
     * @param shift The number of bits to shift.
     * @return A new BigNum representing the result of the shift.
     */
    BigNum operator<<(uint32_t shift) const;

    /**
     * @brief Right shift operator for shifting the BigNum by a certain number of bits to the right.
     * @param shift The number of bits to shift.
     * @return A new BigNum representing the result of the shift.
     */
    BigNum operator>>(uint32_t shift) const;

    // Comparison Operators

    /**
     * @brief Less-than operator for comparing two BigNum objects.
     * @param other The other BigNum object.
     * @return True if this BigNum is smaller than the other, otherwise false.
     */
    bool operator<(const BigNum& other) const;

    /**
     * @brief Greater-than-or-equal-to operator for comparing two BigNum objects.
     * @param other The other BigNum object.
     * @return True if this BigNum is greater than or equal to the other, otherwise false.
     */
    bool operator>=(const BigNum& other) const;

    /**
     * @brief Equality operator for comparing two BigNum objects.
     * @param other The other BigNum object.
     * @return True if both BigNums are equal, otherwise false.
     */
    bool operator==(const BigNum& other) const;

    /**
     * @brief Equality operator for comparing this BigNum with a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNum is equal to the integer, otherwise false.
     */
    bool operator==(uint32_t num) const;

    /**
     * @brief Inequality operator for comparing two BigNum objects.
     * @param other The other BigNum object.
     * @return True if the BigNums are not equal, otherwise false.
     */
    bool operator!=(const BigNum& other) const;

    /**
     * @brief Inequality operator for comparing this BigNum with a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNum is not equal to the integer, otherwise false.
     */
    bool operator!=(uint32_t num) const;

    /**
     * @brief Less-than-or-equal-to operator for comparing two BigNum objects.
     * @param other The other BigNum object.
     * @return True if this BigNum is less than or equal to the other, otherwise false.
     */
    bool operator<=(const BigNum& other) const;

    /**
     * @brief Less-than-or-equal-to operator for comparing this BigNum with a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNum is less than or equal to the integer, otherwise false.
     */
    bool operator<=(uint32_t num) const;

    /**
     * @brief Greater-than operator for comparing two BigNum objects.
     * @param other The other BigNum object.
     * @return True if this BigNum is greater than the other, otherwise false.
     */
    bool operator>(const BigNum& other) const;

    /**
     * @brief Greater-than operator for comparing this BigNum with a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return True if this BigNum is greater than the integer, otherwise false.
     */
    bool operator>(uint32_t num) const;

    // Bitwise Operators

    /**
     * @brief Bitwise AND operator for performing a bitwise AND operation with a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return The result of the bitwise AND operation.
     */
    int operator&(uint32_t num) const;

    /**
     * @brief Bitwise OR operator for performing a bitwise OR operation with a 32-bit unsigned integer.
     * @param num The 32-bit unsigned integer.
     * @return The result of the bitwise OR operation.
     */
    int operator|(uint32_t num) const;

    /**
     * @brief Outputs the BigNum to an output stream (e.g., std::cout).
     * @param out The output stream.
     * @param num The BigNum to be output.
     * @return The output stream with the BigNum written to it.
     */
    friend std::ostream& operator<<(std::ostream& out, const BigNum& num);

    /**
     * @brief Converts the BigNum to a 64-bit unsigned integer.
     * @return The 64-bit unsigned integer representing the BigNum.
     */
    uint64_t to_ulong() const;

    /**
     * @brief Converts the BigNum to a character representation.
     * @return The BigNum as a character.
     */
    uint64_t toChar() const;

    /**
     * @brief Gets the number of 32-bit units that are currently being used in the BigNum.
     * @return The number of full units in the BigNum.
     */
    int getSizeThatIsFull() const;

    /**
     * @brief Gets the length of the BigNum in bits.
     * @return The number of bits used by the BigNum.
     */
    int bit_length() const;
};
