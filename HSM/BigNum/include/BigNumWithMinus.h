#pragma once
#include "BigNum.h"

class BigNumWithMinus : public BigNum {// BigNumWithMinus inherits from BigNum. BigNumWithMinus is fixed to max size
public:
    BigNumWithMinus() : BigNum(MAX_SIZE * UINT_T_SIZE) {}

    BigNumWithMinus(std::string num, int numBase = 16) : BigNum(MAX_SIZE * UINT_T_SIZE) {
        if (num[0] == '-') {
            num = num.substr(1);
            BigNum tmp(num, numBase);
            *this = BigNumWithMinus(tmp);
            setToMinus();
        } else {
            BigNum tmp(num, numBase);
            *this = BigNumWithMinus(tmp);
        }
    }

    BigNumWithMinus(const BigNum& other) : BigNum(MAX_SIZE * UINT_T_SIZE) {
        for (int i = 0; i < other.size; ++i) {
            this->data[i] = other.data[i];
        }
    }

    BigNumWithMinus(int bit_size) : BigNum(MAX_SIZE * UINT_T_SIZE) {}

    BigNumWithMinus(uint32_t num, int bit_size) : BigNum(num, MAX_SIZE * UINT_T_SIZE) {}

    void setToMinus();

    void setToPlus();

    bool operator<(const BigNumWithMinus& other) const;

    bool operator<(uint32_t num) const;
    bool operator>(const BigNumWithMinus& other) const ;

    bool operator>(uint32_t num) const;

    bool operator<=(const BigNumWithMinus& other) const;

    bool operator<=(uint32_t num) const;

    bool operator>=(const BigNumWithMinus& other) const;

    bool operator>=(uint32_t num) const;

    BigNumWithMinus operator-(const BigNumWithMinus& other) const;
    BigNumWithMinus& operator-=(const BigNumWithMinus& other);

    BigNumWithMinus operator*(const BigNumWithMinus& other) const ;

    BigNumWithMinus& operator*=(const BigNumWithMinus& other) ;
    BigNumWithMinus operator/(const BigNumWithMinus& other) const ;

    BigNumWithMinus operator/(uint32_t num) const ;
    BigNumWithMinus& operator/=(const BigNumWithMinus& other) ;

    BigNumWithMinus operator%(const BigNumWithMinus& other) const ;

    BigNumWithMinus operator%(uint32_t num) const ;
    bool isMinus() const ;

    void print() const ;

    std::string toString() const ;
};