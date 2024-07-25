
#include "BigNum.h"

using namespace std;








// Print the number (for testing purposes)
void BigNum::print() const {
	bool leading_zero = true;
	for (int i = size - 1; i >= 0; --i) {
		if (data[i] != 0 || !leading_zero) {
			leading_zero = false;
			std::cout << std::hex << data[i];
		}
	}
	std::cout << std::endl;
}

std::string BigNum::toString() const {
	std::stringstream result;
	bool leading_zero = true;

	for (int i = size - 1; i >= 0; --i) {
		// Convert to hex and handle leading zeros
		std::stringstream part;
		part << std::hex << data[i];

		if (leading_zero) {
			// Find the first non-zero part
			std::string part_str = part.str();
			auto first_non_zero = part_str.find_first_not_of('0');
			if (first_non_zero != std::string::npos) {
				result << part_str.substr(first_non_zero);
				leading_zero = false;
			}
		}
		else {
			// Append remaining parts with leading zeros
			result << std::setfill('0') << std::setw(8) << part.str();
		}
	}

	// If the result is empty, it means the number is zero
	if (result.str().empty()) {
		return "0";
	}

	return result.str();
}


// Example of addition operation
BigNum BigNum::operator+(const BigNum& other) const {
	int max_size = std::max(size, other.size) + 1;
	BigNum result(max_size * UINT_T_SIZE);
	uint64_t carry = 0;
	int i = 0;
	for (; i < size && i < other.size; ++i) {
		uint64_t temp = static_cast<uint64_t>(data[i]) + other.data[i] + carry;
		result.data[i] = static_cast<uint32_t>(temp);
		carry = temp >> UINT_T_SIZE;
	}
	for (; i < size; ++i) {
		uint64_t temp = static_cast<uint64_t>(data[i]) + carry;
		result.data[i] = static_cast<uint32_t>(temp);
		carry = temp >> UINT_T_SIZE;
	}
	for (; i < other.size; ++i) {
		uint64_t temp = static_cast<uint64_t>(other.data[i]) + carry;
		result.data[i] = static_cast<uint32_t>(temp);
		carry = temp >> UINT_T_SIZE;
	}
	if (carry) {
		result.data[i] = static_cast<uint32_t>(carry);
	}
	return result;
}

BigNum BigNum::operator+(int num) const {
	return *this + BigNum(num , 32);
}

BigNum BigNum::operator+=(const BigNum& other) {
	*this = *this + other;
	return *this;
}

BigNum BigNum::operator+=(int num) {
	*this = *this + num;
	return *this;
}



// Subtraction operator
BigNum BigNum::operator-(const BigNum& other) const {
	BigNum result(size* UINT_T_SIZE);
	int64_t borrow = 0;
	int i = 0;
	for (; i < size && i < other.size; ++i) {
		int64_t temp = static_cast<int64_t>(data[i]) - other.data[i] - borrow;
		if (temp < 0) {
			temp += (1LL << UINT_T_SIZE);
			borrow = 1;
		}
		else {
			borrow = 0;
		}
		result.data[i] = static_cast<uint32_t>(temp);
	}
	if (borrow && i < size) {
		result.data[i] = data[i] - borrow;
		borrow = 0;
		i++;
	}
	for (; i < size; ++i) {
		result.data[i] = data[i];
	}
	
	return result;
}

BigNum BigNum::operator-(int num) const {
	return *this - BigNum(num , 32);
}

BigNum BigNum::operator-=(const BigNum& other) {
	*this = *this - other;
	return *this;
}

BigNum BigNum::operator-=(int num) {
	*this = *this - num;
	return *this;
}






BigNum BigNum::operator*(const BigNum& other) const {
	BigNum result(this->size * UINT_T_SIZE + other.size * UINT_T_SIZE);
	for (int i = 0; i < size; ++i) {
		uint64_t carry = 0;
		for (int j = 0; (j < other.size || carry != 0) && i + j < result.size; ++j) {
			uint64_t temp = result.data[i + j] +
				static_cast<uint64_t>(data[i]) * (j < other.size ? other.data[j] : 0) +
				carry;
			result.data[i + j] = static_cast<uint32_t>(temp);
			carry = temp >> UINT_T_SIZE;
		}
	}
	return result;
}

BigNum BigNum::operator*(int num) const {
	return *this * BigNum(num,32);
}


BigNum BigNum::operator*=(const BigNum& other) {
	*this = *this * other;
	return *this;
}

BigNum BigNum::operator*=(int num) {
	*this = *this * num;
	return *this;
}




BigNum BigNum::operator/(const BigNum& other) const {
	if (other == BigNum("0")) {
		throw std::runtime_error("Division by zero error");
	}

	BigNum dividend = *this;
	BigNum divisor = other;
	BigNum quotient(size * UINT_T_SIZE);
	BigNum remainder(size * UINT_T_SIZE);

	for (int i = dividend.size * UINT_T_SIZE - 1; i >= 0; --i) {
		remainder = remainder << 1;
		remainder.data[0] |= (dividend.data[i / UINT_T_SIZE] >> (i % UINT_T_SIZE)) & 1;

		if (remainder >= divisor) {
			remainder = remainder - divisor;
			quotient.data[i / UINT_T_SIZE] |= (1 << (i % UINT_T_SIZE));
		}
	}

	return quotient;
}


BigNum BigNum::operator/(int num) const {
	return *this / BigNum(num,32);
}


BigNum BigNum::operator/=(const BigNum& other) {
	*this = *this / other;
	return *this;
}

BigNum BigNum::operator/=(int num) {
	*this = *this / num;
	return *this;
}

// Modulus operator
// 
 BigNum BigNum::operator%(const BigNum& other) const {
    if (other == BigNum("0")) {
        throw std::runtime_error("Division by zero error");
    }

    BigNum dividend = *this;
    BigNum divisor = other;
    BigNum remainder(size * UINT_T_SIZE);

    for (int i = dividend.size * UINT_T_SIZE - 1; i >= 0; --i) {
        remainder = remainder << 1;
        remainder.data[0] |= (dividend.data[i / UINT_T_SIZE] >> (i % UINT_T_SIZE)) & 1;

        if (remainder >= divisor) {
            remainder = remainder - divisor;
        }
    }

    return remainder;
}


BigNum BigNum::operator%(int num) const {
	return *this % BigNum(num,32);
}



// Left shift operator
BigNum BigNum::operator<<(int shift) const {
	int k = shift / 32;
	int add = size > 0 ? data[size - 1] & 1 : 1;
	BigNum result(size * UINT_T_SIZE + k + add);

	shift %= 32;
	for (int i = size - 1; i >= 0; --i) {
		if (i - k >= 0) {
			result.data[i] = data[i - k] << shift;
			if (i - k - 1 >= 0 && shift != 0) {
				result.data[i] |= data[i - k - 1] >> (32 - shift);
			}
		}
	}
	return result;
}

// Right shift operator
BigNum BigNum::operator>>(int shift) const {
	BigNum result(size * UINT_T_SIZE);
	int k = shift / 32;
	shift %= 32;
	for (int i = 0; i < size; ++i) {
		if (i + k < size) {
			result.data[i] = data[i + k] >> shift;
			if (i + k + 1 < size && shift != 0) {
				result.data[i] |= data[i + k + 1] << (32 - shift);
			}
		}
	}
	return result;
}

// Comparison operators
bool BigNum::operator<(const BigNum& other) const {
	if (size != other.size) {
		if (size < other.size) {
			for (int i = size - 1; i >= other.size; --i) {
				if (data[i] != 0) {
					return true;
				}
			}
		}
		else {
			for (int i = other.size - 1; i >= size; --i) {
				if (other.data[i] != 0) {
					return false;
				}
			}
		}
	}

	int i = std::min(size, other.size) - 1;
	for (; i >= 0; --i) {
		if (data[i] < other.data[i]) {
			return true;
		}
		else if (data[i] > other.data[i]) {
			return false;
		}
	}
	return false;
}

bool BigNum::operator>=(const BigNum& other) const {
	return !(*this < other);
}
bool BigNum::operator ==(const BigNum& other) const {
	int i = 0;
	for (; i < size && i < other.size; ++i) {
		if (data[i] != other.data[i]) return false;
	}
	for (; i < size; ++i) {
		if (data[i] != 0) return false;
	}
	for (; i < other.size; ++i) {
		if (other.data[i] != 0) return false;
	}
	return true;
}




bool BigNum::operator==(int num) const {
	return *this == BigNum(num,32);
}


bool BigNum::operator!=(const BigNum& other) const {
	return !(*this == other);
}

bool BigNum::operator!=(int num) const {
	return !(*this == BigNum(num,32));
}


bool BigNum::operator<=(const BigNum& other) const {

	if (*this < other) {
		return true;
	}
	else if (*this == other) {
		return true;
	}
	else {
		return false;
	}
}

bool BigNum::operator<=(int num) const {
	return *this <= BigNum(num,32);
}


bool BigNum::operator>(const BigNum& other) const {
	return !(*this <= other);
}

bool BigNum::operator>(int num) const {
	return *this > BigNum(num,32);
}


int BigNum::operator&(int num) const {
	return this->data[0] & num;
}



