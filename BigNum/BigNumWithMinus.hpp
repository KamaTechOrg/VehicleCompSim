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

    void setToMinus() {
        for (auto& i : data) {
            i = ~i;
        }
        *this += 1;
    }

    void setToPlus() {
        for (auto& i : data) {
            i = ~i;
        }
        *this += 1;
    }

    bool operator<(const BigNumWithMinus& other) const {
        if (this->isMinus() && !other.isMinus()) {
            return true;
        }
        if (!this->isMinus() && other.isMinus()) {
            return false;
        }
        return BigNum::operator<(other);
    }

    bool operator>(const BigNumWithMinus& other) const {
        return !(*this < other) && !(*this == other);
    }

    bool operator<=(const BigNumWithMinus& other) const {
        return !(*this > other);
    }

    bool operator>=(const BigNumWithMinus& other) const {
        return !(*this < other);
    }

    BigNumWithMinus operator-(const BigNumWithMinus& other) const {
        BigNumWithMinus tmp = other;
        tmp.setToMinus();
        return *this + tmp;
    }

    BigNumWithMinus& operator-=(const BigNumWithMinus& other) {
        *this = *this - other;
        return *this;
    }

    BigNumWithMinus operator*(const BigNumWithMinus& other) const {
        BigNumWithMinus tmpOther = other;
        if (tmpOther.isMinus()) {
            tmpOther.setToPlus();
        }
        BigNumWithMinus toReturn = *this;
        if (toReturn.isMinus()) {
            toReturn.setToPlus();
        }
        toReturn.BigNum::operator*=(tmpOther);

        if (this->isMinus() ^ other.isMinus()) {
            toReturn.setToMinus();
        }
        return toReturn;
    }

    BigNumWithMinus& operator*=(const BigNumWithMinus& other) {
        *this = *this * other;
        return *this;
    }

    BigNumWithMinus operator/(const BigNumWithMinus& other) const {
        BigNumWithMinus tmpOther = other;
        if (tmpOther.isMinus()) {
            tmpOther.setToPlus();
        }
        BigNumWithMinus toReturn = *this;
        if (toReturn.isMinus()) {
            toReturn.setToPlus();
        }
        toReturn.BigNum::operator/=(tmpOther);
        if (this->isMinus() ^ other.isMinus()) {
            toReturn.setToMinus();
        }
        return toReturn;
    }

    BigNumWithMinus operator/(uint32_t num) const {
        return *this / BigNumWithMinus(num, 32);
    }

    BigNumWithMinus& operator/=(const BigNumWithMinus& other) {
        *this = *this / other;
        return *this;
    }

    BigNumWithMinus operator%(const BigNumWithMinus& other) const {
        BigNumWithMinus tmpOther = other;
        if (tmpOther.isMinus()) {
            tmpOther.setToPlus();
        }
        BigNumWithMinus toReturn = *this;
        if (toReturn.isMinus()) {
            toReturn.setToPlus();
        }
        toReturn = toReturn.BigNum::operator%(tmpOther);
        if (this->isMinus() && other.isMinus()) {
            toReturn.setToMinus();
        }
        else if(this->isMinus() ^ other.isMinus()) {
            toReturn -= tmpOther;
            if(this->isMinus()) {
                toReturn.setToPlus();            
            }
        }
        
        return toReturn;
    }

    BigNumWithMinus operator%(uint32_t num) const {
        return *this % BigNumWithMinus(num, 32);
    }

    bool isMinus() const {
        return data[size - 1] & (1 << (UINT_T_SIZE - 1));
    }

    void print() const {
        std::cout << toString() << std::endl;
    }

    std::string toString() const {
        if (!isMinus()) {
            return BigNum::toString();
        } else {
            BigNumWithMinus tmp = *this;
            tmp.setToPlus();
            return "-" + tmp.BigNum::toString();
        }
    }
};
