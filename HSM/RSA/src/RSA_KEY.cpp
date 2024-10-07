#include "RSA_KEY.h"
#include <random>
#include <iostream>
#include <string.h>

    HSM::HSM_STATUS RSA_KEY::generateKeys(std::vector<u_int8_t> &public_key, std::vector<u_int8_t> &private_key, int bits)
{
	BigNum publicKey, privateKey, modulus;
	generate_keys(publicKey, privateKey, modulus, bits);
	if(publicKey == 0 || privateKey == 0 || modulus == 0){
		return HSM::HSM_STATUS::HSM_InternalErr;
	}
	std::vector<u_int8_t> public_key1 = publicKey.toVectorChar();
	std::vector<u_int8_t> private_key1 = privateKey.toVectorChar();
	std::vector<u_int8_t> modulus_str = modulus.toVectorChar();
	modulus_str.emplace_back('P');
	std::vector<u_int8_t> result1 = modulus_str;
	result1.insert(result1.end(), public_key1.begin(), public_key1.end());

	std::vector<u_int8_t> result2 = modulus_str;
	result2.insert(result2.end(), private_key1.begin(), private_key1.end());

	public_key = result1;
	private_key = result2;
	


	return HSM::HSM_STATUS::HSM_Good;
}

void RSA_KEY::generate_keys(BigNum &publicKey, BigNum &privateKey, BigNum &modulus, int bits)
{
	BigNum p = generateLargePrime(bits / 2);
	BigNum q = generateLargePrime(bits / 2);

	BigNum n = p * q;
	BigNum phi = (p - 1) * (q - 1);
	BigNum e("10001"); // Common choice for e

	while (gcd(e, phi) != 1)
	{
		e = generateLargePrime(16);
	}

	BigNum d = modInverse(e, phi);
	publicKey = e;
	privateKey = d;
	modulus = n;
}

bool RSA_KEY::isPrime(const BigNum &n, int k)
{

	if (n <= 1 || n == 4)
		return false;
	if (n <= 3)
		return true;

	BigNum d = n - 1;
	while (d % 2 == 0)
		d /= 2;

	for (int i = 0; i < k; ++i)
	{
		if (!millerTest(d, n))
			return false;
	}
	return true;
}

BigNum RSA_KEY::power(const BigNum &x, const BigNum &y, const BigNum &p)
{
	BigNum res(p.size * BigNum::UINT_T_SIZE);
	res += 1;
	BigNum base = x % p;
	BigNum exp = y;

	while (exp > 0)
	{
		if ((exp.data[0] & 1) == 1)
		{ 
			res = (res * base) % p;
		}
		exp = exp >> 1;
		base = (base * base) % p;
	}
	return res;
}

bool RSA_KEY::millerTest(const BigNum &d, const BigNum &n)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	BigNum nMinusTwo = n - 2;

	std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());
	BigNum a(std::to_string(dis(gen) % (nMinusTwo.to_ulong() - 2) + 2));

	BigNum x = RSA_KEY::power(a, d, n);

	if (x == 1 || x == n - 1)
	{
		return true;
	}

	BigNum tempD = d;
	while (tempD != n - 1)
	{
		x = (x * x) % n;
		tempD = tempD * 2;

		if (x == 1)
			return false;
		if (x == n - 1)
			return true;
	}

	return false;
}

BigNum RSA_KEY::generateRandomNumber(int bits)
{
	std::vector<u_int8_t> bits_str = generateRandomBits(bits);
	BigNum number(bits_str);
	// Ensure the number is odd
	number.data[0] |= 1;
	return number;
}

std::vector<u_int8_t> RSA_KEY::generateRandomBits(size_t length)
{
	if (length < 16)
		length = 16;
	// Calculate the number of bytes needed
	size_t numBytes = (length + 7) / 8; // Round up to the nearest byte

	// Open /dev/urandom for reading
	std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
	if (!urandom)
	{
		throw std::runtime_error("Failed to open /dev/urandom");
	}

	// Create a buffer to hold the random bytes
	std::vector<unsigned char> buffer(numBytes);
	urandom.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

	if (!urandom)
	{
		throw std::runtime_error("Failed to read from /dev/urandom");
	}
	// close(urandom);
	urandom.close();

	// Convert the bytes to a string
	std::ostringstream oss;
	for (unsigned char byte : buffer)
	{
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
	}

	// Trim the string to the specified length
	std::string binaryString = oss.str();
	return std::vector<u_int8_t>(binaryString.begin(), binaryString.end());
}

BigNum RSA_KEY::generateLargePrime(int bits)
{
	BigNum prime;
	int k = 40; // Number of iterations for Miller-Rabin test
	do
	{
		prime = generateRandomNumber(bits);
		// std::cout << "Prime: " << prime << std::endl;
	} while (!isPrime(prime, k));
	return prime;
}

BigNum RSA_KEY::gcd(BigNum a, BigNum b)
{
	while (b != 0)
	{
		BigNum t = b;
		b = a % b;
		a = t;
	}
	return a;
}

BigNum RSA_KEY::modInverse(BigNum a1, BigNum m1)
{
	BigNumWithMinus a(a1);
	BigNumWithMinus m(m1);
	BigNumWithMinus m0 = m;
	BigNumWithMinus t, q;
	BigNumWithMinus x0("0"), x1("1");

	if (m == 1)
		return BigNum("0");

	while (a > 1)
	{
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

