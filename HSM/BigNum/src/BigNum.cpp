
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

BigNum::BigNum(std::vector<u_char> num, int numBase) : size((num.size() / 8) + 1)
{
	if (size > MAX_SIZE)
		size = MAX_SIZE;
	data.resize(size, 0);
	int num_len = num.size();
	std::reverse(num.begin(), num.end());

	for (int i = 0; i < num_len; i += 8)
	{
		std::vector<u_char> part(num.begin() + i, num.begin() + std::min(i + 8, num_len));
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

std::vector<u_char> BigNum::toString() const
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
		return {};
	}

	return std::vector<u_char>(result.str().begin(), result.str().end());
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

BigNum BigNum::operator/(const BigNum &other) const
{
	if (other == BigNum(0, 1))
	{
		throw std::runtime_error("Division by zero error");
	}

	BigNum dividend = *this;
	BigNum divisor = other;
	BigNum quotient(size * UINT_T_SIZE);
	BigNum remainder(size * UINT_T_SIZE);

	for (int i = dividend.size * UINT_T_SIZE - 1; i >= 0; --i)
	{
		remainder = remainder << 1;
		remainder.data[0] |= (dividend.data[i / UINT_T_SIZE] >> (i % UINT_T_SIZE)) & 1;

		if (remainder >= divisor)
		{
			remainder = remainder - divisor;
			quotient.data[i / UINT_T_SIZE] |= (1 << (i % UINT_T_SIZE));
		}
	}

	return quotient;
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
//     int k = n.getSizeThatIsFull() * BigNum::UINT_T_SIZE;

//     BigNum q = x >> (k - 1);  // q = x / 2^k
//     q = q * mu;               // q = q * mu
//     q = q >> (k + 1);         // q = q / 2^(k+1)

//     BigNum r1 = q * n;        // r1 = q * n
//     BigNum r = x - r1;   // r = x - q * n

//     while (r >= n) {
//         r = r - n;            // Correct if r >= n
//     }

//     return r;
// }

int compare(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b)
{
	if (a.size() > b.size())
		return 1;
	if (a.size() < b.size())
		return -1;

	for (size_t i = a.size(); i > 0; --i)
	{
		if (a[i - 1] > b[i - 1])
			return 1;
		if (a[i - 1] < b[i - 1])
			return -1;
	}

	return 0;
}
void subtract(std::vector<uint32_t> &a, const std::vector<uint32_t> &b)
{
	uint64_t borrow = 0;
	for (size_t i = 0; i < b.size(); ++i)
	{
		uint64_t temp = uint64_t(a[i]) - b[i] - borrow;
		a[i] = static_cast<uint32_t>(temp);
		borrow = (temp >> 32) & 1;
	}
	for (size_t i = b.size(); i < a.size() && borrow; ++i)
	{
		uint64_t temp = uint64_t(a[i]) - borrow;
		a[i] = static_cast<uint32_t>(temp);
		borrow = (temp >> 32) & 1;
	}
	// Remove leading zeros
	while (a.size() > 1 && a.back() == 0)
		a.pop_back();
}

// Helper function to shift a large number to the right by a certain number of bits
std::vector<uint32_t> shift_right(const std::vector<uint32_t> &num, int bits)
{
	std::vector<uint32_t> result(num.size(), 0);
	uint64_t temp = 0;
	for (size_t i = num.size(); i > 0; --i)
	{
		temp = (temp << 32) | num[i - 1];
		result[i - 1] = static_cast<uint32_t>(temp >> bits);
		temp &= ((uint64_t(1) << bits) - 1);
	}
	return result;
}

// Helper function to multiply two large numbers
std::vector<uint32_t> multiply(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b)
{
	std::vector<uint32_t> result(a.size() + b.size(), 0);
	for (size_t i = 0; i < a.size(); ++i)
	{
		uint64_t carry = 0;
		for (size_t j = 0; j < b.size() || carry; ++j)
		{
			uint64_t sum = uint64_t(result[i + j]) + uint64_t(a[i]) * (j < b.size() ? b[j] : 0) + carry;
			result[i + j] = static_cast<uint32_t>(sum);
			carry = sum >> 32;
		}
	}
	return result;
}
BigNum barrett_reduce(const BigNum &large_number, const BigNum &mod)
{

	size_t k = mod.getSizeThatIsFull();

	// Step 1: Precompute mu = floor(2^(2k) / mod), where k = mod.size()
	std::vector<uint32_t> mu = shift_right(multiply({0, 1}, {0, 1}), 32 * mod.getSizeThatIsFull() - 1);

	// Step 2: If large_number < mod, no need to reduce
	if (large_number < mod)
		return large_number;

	// Step 3: q1 = floor(large_number / 2^(k-1))
	std::vector<uint32_t> q1 = shift_right(large_number.data, 32 * (k - 1));

	// Step 4: q2 = q1 * mu
	std::vector<uint32_t> q2 = multiply(q1, mu);

	// Step 5: q3 = floor(q2 / 2^(k+1))
	std::vector<uint32_t> q3 = shift_right(q2, 32 * (k + 1));

	// Step 6: r1 = large_number mod 2^(k+1)
	std::vector<uint32_t> r1(large_number.data.begin(), large_number.data.begin() + std::min(large_number.getSizeThatIsFull(), mod.getSizeThatIsFull() + 1));

	// Step 7: r2 = q3 * mod mod 2^(k+1)
	std::vector<uint32_t> r2 = multiply(q3, mod.data);
	r2.resize(mod.getSizeThatIsFull() + 1);

	// Step 8: r = r1 - r2
	subtract(r1, r2);

	// Step 9: If r >= mod, subtract mod from r
	while (compare(r1, mod.data) >= 0)
	{
		subtract(r1, mod.data);
	}
	BigNum toReturn(1, 1);
	toReturn.data = r1;
	toReturn.size = r1.size();
	return toReturn;
}

BigNum BigNum::operator%(const BigNum &other) const
{
	if (*this < other)
	{
		return *this;
	}
	return barrett_reduce(*this, other);
}

// Modulus operator
// BigNum barrett_reduce(const BigNum& x, const BigNum& n, const BigNum& mu) {
// 	BigNum tmp(n);
//     int k = 0;
// 	while(tmp > 0) {
// 		tmp = tmp >> 1;
// 		++k;
// 	}
//     BigNum q = x >> (k - 1);  // q = x / 2^k
//     q = q * mu;               // q = q * mu
//     q = q >> (k + 1);         // q = q / 2^k

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

//     BigNum mu(1, this->size);
//     // Compute mu = 2^(2k) / n
//     BigNum two_k = BigNum(1, this->size) << (2 * other.getSizeThatIsFull() * UINT_T_SIZE);
//     mu = two_k / other;  // Calculate mu

//     BigNum remainder = barrett_reduce(*this, other, mu);

//     return remainder; // Return the remainder
// }

// BigNum BigNum::operator%(const BigNum &other) const
// {
// 	if (other == BigNum("0"))
// 	{
// 		throw std::runtime_error("Division by zero error");
// 	}

// 	BigNum dividend = *this;
// 	BigNum divisor = other;
// 	BigNum remainder(size * UINT_T_SIZE);

// 	for (int i = dividend.size * UINT_T_SIZE - 1; i >= 0; --i)
// 	{
// 		remainder = remainder << 1;
// 		remainder.data[0] |= (dividend.data[i / UINT_T_SIZE] >> (i % UINT_T_SIZE)) & 1;

// 		if (remainder >= divisor)
// 		{
// 			remainder = remainder - divisor;
// 		}
// 	}

// 	return remainder;
// }

BigNum BigNum::operator%(uint32_t num) const
{
	return *this % BigNum(num, 32);
}

// Left shift operator
BigNum BigNum::operator<<(uint32_t shift) const
{
	int k = shift / 32;
	int add = size > 0 ? data[size - 1] & 1 : 1;
	BigNum result((size + k) * UINT_T_SIZE + add);
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
