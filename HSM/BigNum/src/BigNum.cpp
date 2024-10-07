
#include "BigNum.h"
#include <cstdint>
using namespace std;

BigNum::BigNum(int bit_size)
{
	size = (bit_size / UINT_T_SIZE) + (bit_size % UINT_T_SIZE == 0 ? 0 : 1);
	if (size == 0)
	{
		size++;
	}
	if (size > MAX_SIZE)
	{
		size = MAX_SIZE;
	}
	data.resize(size, 0);
}

BigNum::BigNum(uint64_t num, int bit_size) : size(bit_size / UINT_T_SIZE + 1)
{
	data.resize(size, 0);
	data[0] = static_cast<uint32_t>(num);
	data[1] = static_cast<uint32_t>(num >> 32);
}

BigNum::BigNum(std::string num, int numBase) : size((num.size() / 8) + 1)
{
	if (size > MAX_SIZE)
		size = MAX_SIZE;
	data.resize(size, 0);
	int num_len = num.size();
	std::reverse(num.begin(), num.end());

	for (int i = 0; i < num_len; i += 8)
	{
		std::string part = num.substr(i, 8);
		std::reverse(part.begin(), part.end());
		if (i / 8 < size)
		{
			data[i / 8] = std::stoul(part, nullptr, numBase);
		}
	}
}

BigNum::BigNum(std::vector<u_int8_t> num, int numBase) : size((num.size() / 8) + 1)
{
	if (size > MAX_SIZE)
		size = MAX_SIZE;
	data.resize(size, 0);
	int num_len = num.size();
	std::reverse(num.begin(), num.end());

	for (int i = 0; i < num_len; i += 8)
	{
		std::vector<u_int8_t> part(num.begin() + i, num.begin() + std::min(i + 8, num_len));
		std::reverse(part.begin(), part.end());
		if (i / 8 < size)
		{
			data[i / 8] = std::stoul(std::string(part.begin(), part.end()), nullptr, numBase);
		}
	}
}
void BigNum::print() const
{
	bool leading_zero = true;
	for (int i = size - 1; i >= 0; --i)
	{
		if (data[i] != 0 || !leading_zero)
		{
			leading_zero = false;
			std::cout << std::hex << data[i];
		}
	}
	std::cout << std::endl;
}

std::string BigNum::toString() const
{
	std::stringstream result;
	bool leading_zero = true;

	for (int i = size - 1; i >= 0; --i)
	{
		std::stringstream part;
		part << std::hex << data[i];

		if (leading_zero)
		{
			string part_str = part.str();
			auto first_non_zero = part_str.find_first_not_of('0');
			if (first_non_zero != string::npos)
			{
				result << part_str.substr(first_non_zero);
				leading_zero = false;
			}
		}
		else
		{
			result << std::setfill('0') << std::setw(8) << part.str();
		}
	}

	if (result.str().empty())
	{
		return "0";
	}

	return result.str();
}

std::vector<u_int8_t> BigNum::toVectorChar() const
{
	std::string str = toString();
	return std::vector<u_int8_t>(str.begin(), str.end());
}

BigNum BigNum::operator+(const BigNum &other) const
{
	int max_size = std::max(size, other.size) + 1;
	BigNum result(max_size * UINT_T_SIZE);
	uint64_t carry = 0;
	int i = 0;
	for (; i < size && i < other.size; ++i)
	{
		uint64_t temp = static_cast<uint64_t>(data[i]) + other.data[i] + carry;
		result.data[i] = static_cast<uint32_t>(temp);
		carry = temp >> UINT_T_SIZE;
	}
	for (; i < size; ++i)
	{
		uint64_t temp = static_cast<uint64_t>(data[i]) + carry;
		result.data[i] = static_cast<uint32_t>(temp);
		carry = temp >> UINT_T_SIZE;
	}
	for (; i < other.size; ++i)
	{
		uint64_t temp = static_cast<uint64_t>(other.data[i]) + carry;
		result.data[i] = static_cast<uint32_t>(temp);
		carry = temp >> UINT_T_SIZE;
	}
	if (carry)
	{
		result.data[i] = static_cast<uint32_t>(carry);
	}
	return result;
}

BigNum BigNum::operator+(uint32_t num) const
{
	return *this + BigNum(num, 32);
}

BigNum BigNum::operator+=(const BigNum &other)
{
	*this = *this + other;
	return *this;
}

BigNum BigNum::operator+=(uint32_t num)
{
	*this = *this + num;
	return *this;
}

// Subtraction operator
BigNum BigNum::operator-(const BigNum &other) const
{
	BigNum result(size * UINT_T_SIZE);
	int64_t borrow = 0;
	int i = 0;
	for (; i < size && i < other.size; ++i)
	{
		int64_t temp = static_cast<int64_t>(data[i]) - other.data[i] - borrow;
		if (temp < 0)
		{
			temp += (1LL << UINT_T_SIZE);
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}
		result.data[i] = static_cast<uint32_t>(temp);
	}
	while (borrow && i < size)
	{
		if (data[i] != 0)
		{
			result.data[i] = data[i] - borrow;
			borrow = 0;
		}
		i++;
	}
	for (; i < size; ++i)
	{
		result.data[i] = data[i];
	}
	if (result > *this)
	{
		return BigNum(0, 32);
	}

	return result;
}

BigNum BigNum::operator-(uint32_t num) const
{
	return *this - BigNum(num, 32);
}

BigNum BigNum::operator-=(const BigNum &other)
{
	*this = *this - other;
	return *this;
}

BigNum BigNum::operator-=(uint32_t num)
{
	*this = *this - num;
	return *this;
}

BigNum BigNum::operator*(const BigNum &other) const
{
	BigNum result(this->size * UINT_T_SIZE + other.size * UINT_T_SIZE);
	for (int i = 0; i < size; ++i)
	{
		uint64_t carry = 0;
		for (int j = 0; (j < other.size || carry != 0) && i + j < result.size; ++j)
		{
			uint64_t temp = result.data[i + j] +
							static_cast<uint64_t>(data[i]) * (j < other.size ? other.data[j] : 0) +
							carry;
			result.data[i + j] = static_cast<uint32_t>(temp);
			carry = temp >> UINT_T_SIZE;
		}
	}
	return result;
}

BigNum BigNum::operator*(uint32_t num) const
{
	return *this * BigNum(num, 32);
}

BigNum BigNum::operator*=(const BigNum &other)
{
	*this = *this * other;
	return *this;
}

BigNum BigNum::operator*=(uint32_t num)
{
	*this = *this * num;
	return *this;
}

// BigNum BigNum::operator/(const BigNum &other) const
// {
// 	if (other == BigNum(0, 1))
// 	{
// 		throw std::runtime_error("Division by zero error");
// 	}

// 	BigNum dividend = *this;
// 	BigNum divisor = other;
// 	BigNum quotient(size * UINT_T_SIZE);
// 	BigNum remainder(size * UINT_T_SIZE);

// 	for (int i = dividend.size * UINT_T_SIZE - 1; i >= 0; --i)
// 	{
// 		remainder = remainder << 1;
// 		remainder.data[0] |= (dividend.data[i / UINT_T_SIZE] >> (i % UINT_T_SIZE)) & 1;

// 		if (remainder >= divisor)
// 		{
// 			remainder = remainder - divisor;
// 			quotient.data[i / UINT_T_SIZE] |= (1 << (i % UINT_T_SIZE));
// 		}
// 	}

// 	return quotient;
// }


BigNum BigNum::operator/(const BigNum &other) const
{
    // Check for division by zero
    if (other == BigNum(0, 1))
    {
        throw std::runtime_error("Division by zero error");
    }

    // Handle the case when the numerator is smaller than the divisor
    if (*this < other)
    {
        return BigNum("0"); // Division result is zero if num < divisor
    }

    BigNum num = *this;        // Copy of the numerator
    BigNum divisor = other;    // Divisor
    BigNum result("0");        // Stores the result (quotient)
    BigNum currentDivisor = divisor;
    BigNum quotient("1");

    // Left shift the divisor to the highest possible multiple less than or equal to num
    while ((currentDivisor << 1) <= num)
    {
        currentDivisor = currentDivisor << 1;
        quotient = quotient << 1;
    }

    // Perform the division by subtracting and accumulating quotient
    while (num >= other)
    {
        if (num >= currentDivisor)
        {
            num = num - currentDivisor;
            result = result + quotient;
        }

        // Right shift to reduce the currentDivisor and quotient for the next step
        currentDivisor = currentDivisor >> 1;
        quotient = quotient >> 1;
    }

    return result;
}


BigNum BigNum::operator/(uint32_t num) const
{
	return *this / BigNum(num, 32);
}

BigNum BigNum::operator/=(const BigNum &other)
{
	*this = *this / other;
	return *this;
}

BigNum BigNum::operator/=(uint32_t num)
{
	*this = *this / num;
	return *this;
}
// BigNum barrett_reduce(const BigNum& x, const BigNum& n, const BigNum& mu) {
//     // Calculate k as the bit-length of n
//     int k = n.bit_length(); // Assuming there's a function bit_length()

//     BigNum q = x >> (k - 1);  // q = x / 2^k
//     q = q * mu;               // q = q * mu
//     q = q >> (k + 1);         // q = q / 2^(k+1)

//     BigNum r = x - (q * n);   // r = x - q * n

//     while (r >= n) {
//         r = r - n;            // Correct if r >= n
//     }

//     return r; 
// }

// BigNum BigNum::operator%(const BigNum& other) const {
//     if (*this < other) {
//         return *this; // If this is less than the modulus, return this
//     }

//     // Compute mu = 2^(2k) / n
//     int k = other.bit_length(); // Assuming other has a bit_length function
//     BigNum two_k = BigNum(1) << (2 * k);  // 2^(2k)
//     BigNum mu = two_k / other;  // Calculate mu

//     BigNum remainder = barrett_reduce(*this, other, mu);

//     return remainder; // Return the remainder
// }



BigNum BigNum::operator%(const BigNum &other) const
{
    // Check for division by zero
    if (other == BigNum("0"))
    {
        throw std::runtime_error("Division by zero error");
    }
    BigNum num = *this;
    
    if (num < other) {
        return num;
    }

    BigNum mod = other;
    BigNum shiftedMod = mod;
    while (shiftedMod <= num)
    {
        mod = shiftedMod;
        shiftedMod = shiftedMod << 1;
    }

    while (num >= other)
    {
        if (num >= mod)
        {
            num = num - mod;
        }
        mod = mod >> 1;
    }

    return num;
}


BigNum BigNum::operator%(uint32_t num) const
{
	return *this % BigNum(num, 32);
}

// Left shift operator
BigNum BigNum::operator<<(uint32_t shift) const
{
	int k = shift / 32;

	int add = size > 0 ? data[size - 1] > 0 ? 1 : 0 : 1;
	BigNum result((size + k + add) * UINT_T_SIZE);
	BigNum temp(*this);
	temp.size = result.size;
	temp.data.resize(temp.size);

	shift %= 32;
	for (int i = temp.size - 1; i >= 0; --i)
	{
		if (i - k >= 0)
		{
			result.data[i] = temp.data[i - k] << shift;
			if (i - k - 1 >= 0 && shift != 0)
			{
				result.data[i] |= temp.data[i - k - 1] >> (32 - shift);
			}
		}
	}
	return result;
}

// Right shift operator
BigNum BigNum::operator>>(uint32_t shift) const
{
	BigNum result(size * UINT_T_SIZE);
	int k = shift / 32;
	shift %= 32;
	for (int i = 0; i < size; ++i)
	{
		if (i + k < size)
		{
			result.data[i] = data[i + k] >> shift;
			if (i + k + 1 < size && shift != 0)
			{
				result.data[i] |= data[i + k + 1] << (32 - shift);
			}
		}
	}
	return result;
}

// Comparison operators
bool BigNum::operator<(const BigNum &other) const
{
	if (size != other.size)
	{
		if (size > other.size)
		{
			for (int i = size - 1; i >= other.size; --i)
			{
				if (data[i] != 0)
				{
					return false;
				}
			}
		}
		else
		{
			for (int i = other.size - 1; i >= size; --i)
			{
				if (other.data[i] != 0)
				{
					return true;
				}
			}
		}
	}

	int i = std::min(size, other.size) - 1;
	for (; i >= 0; --i)
	{
		if (data[i] < other.data[i])
		{
			return true;
		}
		else if (data[i] > other.data[i])
		{
			return false;
		}
	}
	return false;
}

bool BigNum::operator>=(const BigNum &other) const
{
	return !(*this < other);
}
bool BigNum::operator==(const BigNum &other) const
{
	int i = 0;
	for (; i < size && i < other.size; ++i)
	{
		if (data[i] != other.data[i])
			return false;
	}
	for (; i < size; ++i)
	{
		if (data[i] != 0)
			return false;
	}
	for (; i < other.size; ++i)
	{
		if (other.data[i] != 0)
			return false;
	}
	return true;
}

bool BigNum::operator==(uint32_t num) const
{
	return *this == BigNum(num, 32);
}

bool BigNum::operator!=(const BigNum &other) const
{
	return !(*this == other);
}

bool BigNum::operator!=(uint32_t num) const
{
	return !(*this == BigNum(num, 32));
}

bool BigNum::operator<=(const BigNum &other) const
{

	if (*this < other)
	{
		return true;
	}
	else if (*this == other)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BigNum::operator<=(uint32_t num) const
{
	return *this <= BigNum(num, 32);
}

bool BigNum::operator>(const BigNum &other) const
{
	return !(*this <= other);
}

bool BigNum::operator>(uint32_t num) const
{
	return *this > BigNum(num, 32);
}

int BigNum::operator&(uint32_t num) const
{
	return this->data[0] & num;
}

int BigNum::operator|(uint32_t num) const
{
	return this->data[0] | num;
}

uint64_t BigNum::to_ulong() const
{
	return static_cast<uint64_t>(data[0]);
}

uint64_t BigNum::toChar() const
{
	return static_cast<char>(data[0]);
}

int BigNum::getSizeThatIsFull() const
{
	int i = 0;
	while (i < size && data[size - 1 - i] == 0)
	{
		++i;
	}
	return size - i;
}

int BigNum::bit_length() const
{
	BigNum tmp(*this);
	int k = 0;
	while (tmp > 0)
	{
		tmp = tmp >> 1;
		++k;
	}
	return k;
}
