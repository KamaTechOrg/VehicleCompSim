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


//this is a big number class that allows for large numbers
//at any size up to 1024 bits

//the class should be able to do basic arithmetic operations
//such as addition, subtraction, multiplication, etc.



class BigNum {
public:
    static const int MAX_SIZE = 64;
    static const int UINT_T_SIZE = 32;

    int size;
    std::vector<uint32_t> data;
    BigNum() : size(MAX_SIZE) {
		data.resize(size, 0);
    }
    BigNum(int bit_size);

	BigNum(uint64_t num, int bit_size);

    BigNum(std::vector<u_int8_t> num, int numBase = 16);
    BigNum(std::string num, int numBase = 16);
    

    // Print the number (for testing purposes)
    void print() const;

    std::string toString() const;

    std::vector<u_int8_t> toVectorChar() const;


    BigNum operator+(const BigNum& other) const;

    BigNum operator+(uint32_t num) const;

    BigNum operator+=(const BigNum& other);

    BigNum operator+=(uint32_t num);




    BigNum operator*(const BigNum& other) const;

    BigNum operator*(uint32_t num) const;



    BigNum operator*=(const BigNum& other);

    BigNum operator*=(uint32_t num);




    // Subtraction operator
    BigNum operator-(const BigNum& other) const;

    BigNum operator-(uint32_t num) const;

    BigNum operator-=(const BigNum& other);

    BigNum operator-=(uint32_t num);

    // Modulus operator
    BigNum operator%(const BigNum& other) const;

    BigNum operator%(uint32_t num) const;

    BigNum operator/(const BigNum& other) const;

    BigNum operator/(uint32_t num) const;

    BigNum operator/=(const BigNum& other);

    BigNum operator/=(uint32_t num);

    // Left shift operator
    BigNum operator<<(uint32_t shift) const;

    // Right shift operator
    BigNum operator>>(uint32_t shift) const;

    // Comparison operators
    bool operator<(const BigNum& other) const;

    bool operator>=(const BigNum& other) const;
    bool operator==(const BigNum& other) const;

    bool operator==(uint32_t num) const;


    bool operator!=(const BigNum& other) const;

    bool operator!=(uint32_t num) const;

    bool operator<=(const BigNum& other) const;

    bool operator<=(uint32_t num) const;


    bool operator>(const BigNum& other) const;

    bool operator>(uint32_t num) const;

    int operator&(uint32_t num) const;

    int operator|(uint32_t num) const;



	friend std::ostream& operator<<(std::ostream& out, const BigNum& num) {
        std::string str = num.toString();
		out << str;
		return out;
	}

    uint64_t to_ulong() const;
    
    uint64_t toChar() const;

    int getSizeThatIsFull() const;

    int bit_length() const;


};



