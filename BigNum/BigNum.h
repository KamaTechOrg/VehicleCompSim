#pragma once
#include <iostream>
#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>


//this is a big number class that allows for large numbers
//at any size up to 1024 bits

//the class should be able to do basic arithmetic operations
//such as addition, subtraction, multiplication, etc.



class BigNum {
public:
    static const int MAX_SIZE = 32;
    static const int UINT_T_SIZE = 32;

    int size;
    std::vector<uint32_t> data;
    BigNum() : size(MAX_SIZE) {
		data.resize(size, 0);
    }
    BigNum(int bit_size) {
        size = (bit_size / UINT_T_SIZE) + (bit_size % UINT_T_SIZE == 0? 0 : 1);
        if (size == 0)size++;
        if(size > MAX_SIZE)size = MAX_SIZE;

        if(!size) {
			std::cout << "size is 0" << std::endl;
		}
        //size = MAX_SIZE;

        data.resize(size, 0);
    }

	BigNum(uint64_t num, int bit_size) : size(bit_size / UINT_T_SIZE + 1) {
		data.resize(size, 0);
        data[0] = static_cast<uint32_t>(num);
        data[1] = static_cast<uint32_t>(num >> 32);
		
	}

    BigNum(std::string num, int numBase = 16) : size((num.size() / 8) + 1) {
        if (size > MAX_SIZE)size = MAX_SIZE;
        data.resize(size, 0);
        int num_len = num.size();
        std::reverse(num.begin(), num.end());


        for (int i = 0; i < num_len; i += 8) {
            std::string part = num.substr(i, 8);
            std::reverse(part.begin(), part.end());
            if (i / 8 < size) {
                data[i / 8] = std::stoul(part, nullptr, numBase);
            }
        }
    }
   
    

    // Print the number (for testing purposes)
    void print() const;

    std::string toString() const;


    BigNum operator+(const BigNum& other) const;

    BigNum operator+(int num) const;

    BigNum operator+=(const BigNum& other);

	BigNum operator+=(int num);




    BigNum operator*(const BigNum& other) const;

	BigNum operator*(int num) const;



	BigNum operator*=(const BigNum& other);

	BigNum operator*=(int num);


    

    // Subtraction operator
    BigNum operator-(const BigNum& other) const;

	BigNum operator-(int num) const;

	BigNum operator-=(const BigNum& other);

	BigNum operator-=(int num);

    // Modulus operator
    BigNum operator%(const BigNum& other) const;

	BigNum operator%(int num) const;

    BigNum operator/(const BigNum& other) const;

	BigNum operator/(int num) const;

    BigNum operator/=(const BigNum& other);

	BigNum operator/=(int num);

    // Left shift operator
    BigNum operator<<(int shift) const;

    // Right shift operator
    BigNum operator>>(int shift) const;

    // Comparison operators
    bool operator<(const BigNum& other) const;

    bool operator>=(const BigNum& other) const;
    bool operator==(const BigNum& other) const;

    bool operator==(int num) const;


    bool operator!=(const BigNum& other) const;

	bool operator!=(int num) const;

    bool operator<=(const BigNum& other) const;

	bool operator<=(int num) const;


	bool operator>(const BigNum& other) const;

	bool operator>(int num) const;

    int operator&(int num) const;


	friend std::ostream& operator<<(std::ostream& out, const BigNum& num) {
		out << num.toString();
		return out;
	}

};



