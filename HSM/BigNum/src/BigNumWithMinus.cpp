#include "BigNumWithMinus.h"


void BigNumWithMinus::setToMinus()
{
    for (int i = 0; i < size; ++i)
    {
        data[i] = ~data[i];
    }
    *this += 1;
}

void BigNumWithMinus::setToPlus()
{
    for (int i = 0; i < size; ++i)
    {
        data[i] = ~data[i];
    }
    *this += 1;
}

bool BigNumWithMinus::operator<(const BigNumWithMinus &other) const
{
    if (this->isMinus() && !other.isMinus())
    {
        return true;
    }
    if (!this->isMinus() && other.isMinus())
    {
        return false;
    }
    return BigNum::operator<(other);
}

bool BigNumWithMinus::operator<(uint32_t num) const
{
    return *this < BigNumWithMinus(num, MAX_SIZE * UINT_T_SIZE);
}

bool BigNumWithMinus::operator>(const BigNumWithMinus &other) const
{
    return !(*this < other) && !(*this == other);
}

bool BigNumWithMinus::operator>(uint32_t num) const
{
    return *this > BigNumWithMinus(num, MAX_SIZE * UINT_T_SIZE);
}

bool BigNumWithMinus::operator<=(const BigNumWithMinus &other) const
{
    return !(*this > other);
}

bool BigNumWithMinus::operator<=(uint32_t num) const
{
    return *this <= BigNumWithMinus(num, MAX_SIZE * UINT_T_SIZE);
}

bool BigNumWithMinus::operator>=(const BigNumWithMinus &other) const
{
    return !(*this < other);
}

bool BigNumWithMinus::operator>=(uint32_t num) const
{
    return *this >= BigNumWithMinus(num, MAX_SIZE * UINT_T_SIZE);
}

BigNumWithMinus BigNumWithMinus::operator-(const BigNumWithMinus &other) const
{
    BigNumWithMinus tmp = other;
    tmp.setToMinus();
    return *this + tmp;
}

BigNumWithMinus &BigNumWithMinus::operator-=(const BigNumWithMinus &other)
{
    *this = *this - other;
    return *this;
}

BigNumWithMinus BigNumWithMinus::operator*(const BigNumWithMinus &other) const
{
    BigNumWithMinus tmpOther = other;
    if (tmpOther.isMinus())
    {
        tmpOther.setToPlus();
    }
    BigNumWithMinus toReturn = *this;
    if (toReturn.isMinus())
    {
        toReturn.setToPlus();
    }
    toReturn.BigNum::operator*=(tmpOther);

    if (this->isMinus() ^ other.isMinus())
    {
        toReturn.setToMinus();
    }
    return toReturn;
}

BigNumWithMinus &BigNumWithMinus::operator*=(const BigNumWithMinus &other)
{
    *this = *this * other;
    return *this;
}

BigNumWithMinus BigNumWithMinus::operator/(const BigNumWithMinus &other) const
{
    BigNumWithMinus tmpOther = other;
    if (tmpOther.isMinus())
    {
        tmpOther.setToPlus();
    }
    BigNumWithMinus toReturn = *this;
    if (toReturn.isMinus())
    {
        toReturn.setToPlus();
    }
    toReturn.BigNum::operator/=(tmpOther);
    if (this->isMinus() ^ other.isMinus())
    {
        toReturn.setToMinus();
    }
    return toReturn;
}

BigNumWithMinus BigNumWithMinus::operator/(uint32_t num) const
{
    return *this / BigNumWithMinus(num, 32);
}

BigNumWithMinus &BigNumWithMinus::operator/=(const BigNumWithMinus &other)
{
    *this = *this / other;
    return *this;
}

BigNumWithMinus BigNumWithMinus::operator%(const BigNumWithMinus &other) const
{
    BigNumWithMinus tmpOther = other;
    if (tmpOther.isMinus())
    {
        tmpOther.setToPlus();
    }
    BigNumWithMinus toReturn = *this;
    if (toReturn.isMinus())
    {
        toReturn.setToPlus();
    }
    toReturn = toReturn.BigNum::operator%(tmpOther);
    if (this->isMinus() && other.isMinus())
    {
        toReturn.setToMinus();
    }
    else if (this->isMinus() ^ other.isMinus())
    {
        toReturn -= tmpOther;
        if (this->isMinus())
        {
            toReturn.setToPlus();
        }
    }

    return toReturn;
}

BigNumWithMinus BigNumWithMinus::operator%(uint32_t num) const
{
    return *this % BigNumWithMinus(num, 32);
}

bool BigNumWithMinus::isMinus() const
{
    return data[size - 1] & (1 << (UINT_T_SIZE - 1));
}

void BigNumWithMinus::print() const
{
    // Directly convert to string and print
    std::cout << std::string(toString().begin(), toString().end()) << std::endl;
}

std::vector<u_char> BigNumWithMinus::toString() const
{
    if (!isMinus())
    {
        return BigNum::toString();
    }
    else
    {
        BigNumWithMinus tmp = *this;
        tmp.setToPlus();
        std::vector<u_char> toReturn{'-'};
        std::vector<u_char> tmpVec = tmp.BigNum::toString();
        toReturn.insert(toReturn.end(), tmpVec.begin(), tmpVec.end());
        return toReturn;
    }
}
