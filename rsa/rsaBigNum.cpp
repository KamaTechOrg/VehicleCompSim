#include "rsaBigNum.h"
#include <random>
#include <iostream>

void RSABigNum::generate_keys(BigNum& publicKey, BigNum& privateKey, BigNum& modulus, int bits) {
	BigNum p = generateLargePrime(bits / 2);
	std::cout << "Prime p: " << p << std::endl;
	BigNum q = generateLargePrime(bits / 2);
	std::cout << "Prime q: " << q << std::endl;

	BigNum n = p * q;
	BigNum phi = (p - 1) * (q - 1);
	BigNum e = 65537; // Common choice for e

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
	BigNum number(bits);
	for(auto& v : number.data) {
		for(int i = 0; i < BigNum::UINT_T_SIZE; ++i) {
			v = v << 1;
			v += rand() & 1;
		}
	}


	// Ensure the number is odd
	number.data[0] |= 1;

	// Test if the generated number is prime


	std::cout << "Generated random number: " << number << std::endl;
	std::cout << "count: " << count << std::endl;
	return number;
}





//BigNum RSABigNum::generateRandomNumber(int bits) {
//    BigNum number(bits);
//    for (int i = 0; i < number.size; ++i) {
//        number.data[i] = rand();
//    }
//    std::cout << "Generated number: " << number << std::endl;
//    return number;
//}

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
	while (b != BigNum("0", b.size * BigNum::UINT_T_SIZE)) {
		BigNum t = b;
		b = a % b;
		a = t;
	}
	return a;
}

BigNum RSABigNum::modInverse(BigNum a, BigNum m) {
	BigNum m0 = m, t, q;
	BigNum x0("0", m.size * BigNum::UINT_T_SIZE), x1("1", m.size * BigNum::UINT_T_SIZE);

	if (m == BigNum("1", m.size * BigNum::UINT_T_SIZE))
		return BigNum("0", m.size * BigNum::UINT_T_SIZE);

	while (a > BigNum("1", a.size * BigNum::UINT_T_SIZE)) {
		q = a / m;
		t = m;
		m = a % m;
		a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}

	if (x1 < BigNum("0", x1.size * BigNum::UINT_T_SIZE))
		x1 += m0;

	return x1;
}

BigNum RSABigNum::encrypt(const BigNum& message, const BigNum& publicKey, const BigNum& modulus) {
	return power(message, publicKey, modulus);
}

BigNum RSABigNum::decrypt(const BigNum& message, const BigNum& privateKey, const BigNum& modulus) {
	return power(message, privateKey, modulus);
}

std::string RSABigNum::encrypt_string(const std::string& message, const BigNum& publicKey, const BigNum& modulus) {
	std::string result;
	for (char c : message) {
		BigNum encryptedChar = encrypt(BigNum(std::to_string(c), modulus.size * BigNum::UINT_T_SIZE), publicKey, modulus);
		result += encryptedChar.toString() + " ";
	}
	return result;
}

std::string RSABigNum::decrypt_string(const std::string& encrypted_message, const BigNum& privateKey, const BigNum& modulus) {
	std::stringstream ss(encrypted_message);
	std::string item;
	std::string result;
	while (std::getline(ss, item, ' ')) {
		BigNum encryptedChar(item, modulus.size * BigNum::UINT_T_SIZE);
		char decryptedChar = static_cast<char>(decrypt(encryptedChar, privateKey, modulus).to_ulong());
		result += decryptedChar;
	}
	return result;
}
