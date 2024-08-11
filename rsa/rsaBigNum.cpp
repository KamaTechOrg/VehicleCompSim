#include "rsaBigNum.h"
#include <random>
#include <iostream>

void RSABigNum::generate_keys(BigNum& publicKey, BigNum& privateKey, BigNum& modulus, int bits) {
	std::string num1 = "fbc727ead875405abb015014aacf827cfdbcd1f58907640c33d14f0df6c239586ffd55252d4100fe5422d05ffa3b15b1db9ebb1e895cc42049bf0bc28a15095f";
	std::string num2 = "9584fc2295db9bd232e18c6352d22d146e9c26693754f9045b28774886d3275eda82ba271bdfaad6eacc93a0ed37a05eb17a96e1e2b94c1cd5e329bfe95c8309";
	BigNum p(num1);
	// std::cout << "Prime p: " << p << std::endl;
	BigNum q(num2);
	// std::cout << "Prime q: " << q << std::endl;

	BigNum n = p * q;
	BigNum phi = (p - 1) * (q - 1);
	BigNum e("10001"); // Common choice for e

	// Ensure e is coprime with phi
	while (gcd(e, phi) != 1) {
		e = generateLargePrime(16);
		std::cout << "New e: " << e << std::endl;
	}

	BigNum d = modInverse(e, phi);
	publicKey = e;
	privateKey = d;
	modulus = n;
}

bool RSABigNum::isPrime(const BigNum& n, int k) {
	
	if (n <= 1 || n == 4)
		return false;
	if (n <= 3)
		return true;

	BigNum d = n - 1;
	while (d % 2 == 0)
		d /= 2;

	for (int i = 0; i < k; ++i) {
		if (!millerTest(d, n))
			return false;
	}
	return true;
}

BigNum RSABigNum::power(const BigNum& x, const BigNum& y, const BigNum& p) {
	BigNum res(p.size * BigNum::UINT_T_SIZE);
	res += 1;
	BigNum base = x % p;
	BigNum exp = y;

	while (exp > 0) {
		if ((exp.data[0] & 1) == 1) { // If exp is odd
			res = (res * base) % p;
		}
		exp = exp >> 1;
		base = (base * base) % p;
	}
	return res;
}

bool RSABigNum::millerTest(const BigNum& d, const BigNum& n) {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	BigNum nMinusTwo = n - 2;

	std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());
	BigNum a(std::to_string(dis(gen) % (nMinusTwo.to_ulong() - 2) + 2));

	BigNum x = RSABigNum::power(a, d, n);

	if (x == 1 || x == n - 1) {
		return true;
	}

	BigNum tempD = d;
	while (tempD != n - 1) {
		x = (x * x) % n;
		tempD = tempD * 2;

		if (x == 1)
			return false;
		if (x == n - 1)
			return true;
	}

	return false;
}

BigNum RSABigNum::generateRandomNumber(int bits,int& count) {
	// Determine the number of uint32_t values needed
	count++;
	std::string bits_str = generateRandomBits(bits);
	BigNum number(bits_str);
	// Ensure the number is odd
	number.data[0] |= 1;

	// Test if the generated number is prime


	std::cout << "Generated random number: " << number << std::endl;
	std::cout << "count: " << count << std::endl;
	return number;
}




std::string RSABigNum::generateRandomBits(size_t length) {
    // Calculate the number of bytes needed
    size_t numBytes = (length + 7) / 8; // Round up to the nearest byte

    // Open /dev/urandom for reading
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (!urandom) {
        throw std::runtime_error("Failed to open /dev/urandom");
    }

    // Create a buffer to hold the random bytes
    std::vector<unsigned char> buffer(numBytes);
    urandom.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    if (!urandom) {
        throw std::runtime_error("Failed to read from /dev/urandom");
    }
	//close(urandom);
	urandom.close();

	// Convert the bytes to a string
     std::ostringstream oss;
    for (unsigned char byte : buffer) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }

    // Trim the string to the specified length
    std::string binaryString = oss.str();
    return binaryString.substr(0, (length/4)+ 1);
}









BigNum RSABigNum::generateLargePrime(int bits) {
	BigNum prime;
	int count = 0;
	int k = 40; // Number of iterations for Miller-Rabin test
	do {
		prime = generateRandomNumber(bits, count);
	} while (!isPrime(prime, k));
	return prime;
}

BigNum RSABigNum::gcd(BigNum a, BigNum b) {
	while (b != 0) {
		BigNum t = b;
		b = a % b;
		a = t;
	}
	return a;
}

BigNum RSABigNum::modInverse(BigNum a1, BigNum m1) {
	BigNumWithMinus a(a1);
	BigNumWithMinus m(m1);
	BigNumWithMinus m0 = m;
	BigNumWithMinus t, q;
	BigNumWithMinus x0("0"), x1("1");

	if (m == 1)
		return BigNum("0");

	while (a > 1) {
		q = a / m;
		t = m;
		m = a % m;
		a = t;
		t = x0;
		x0 = x1 - (q * x0);
		x1 = t;
	}

	if (x1 < 0)
		x1 += m0;

	return x1;
}

BigNum RSABigNum::encrypt(const BigNum& message, const BigNum& publicKey, const BigNum& modulus) {
	return power(message, publicKey, modulus);
}

BigNum RSABigNum::decrypt(const BigNum& message, const BigNum& privateKey, const BigNum& modulus) {
	return power(message, privateKey, modulus);
}



std::string RSABigNum::encrypt(const std::string& encrypted_message, const BigNum& privateKey, const BigNum& modulus) {
	std::stringstream ss(encrypted_message);
	std::string item;
	std::string result;
	while (std::getline(ss, item, ' ')) {
		BigNum encryptedChar(item);
		result += encrypt(encryptedChar, privateKey, modulus).toString();
	}
	return result;
}


std::string RSABigNum::decrypt(const std::string& encrypted_message, const BigNum& privateKey, const BigNum& modulus) {
	return encrypt(encrypted_message, privateKey, modulus);
}
