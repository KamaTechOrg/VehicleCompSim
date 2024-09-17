#include "RSA.h"
#include <random>
#include <iostream>
#include <string.h>

std::pair<std::vector<u_char>, std::vector<u_char>> RSA::generateKeys(ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
	BigNum publicKey, privateKey, modulus;
	generate_keys(publicKey, privateKey, modulus, bits / 2);
	std::vector<u_char> public_key = publicKey.toString();
	std::vector<u_char> private_key = privateKey.toString();
	std::vector<u_char> modulus_str = modulus.toString();
	modulus_str.emplace_back('P');
	std::vector<u_char> result1 = modulus_str;
	result1.insert(result1.end(), public_key.begin(), public_key.end());

	std::vector<u_char> result2 = modulus_str;
	result2.insert(result2.end(), private_key.begin(), private_key.end());

	return std::pair<std::vector<u_char>, std::vector<u_char>>(result1, result2);
}

void RSA::generate_keys(BigNum &publicKey, BigNum &privateKey, BigNum &modulus, int bits)
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

bool RSA::isPrime(const BigNum &n, int k)
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

BigNum RSA::power(const BigNum &x, const BigNum &y, const BigNum &p)
{
	BigNum res(p.size * BigNum::UINT_T_SIZE);
	res += 1;
	BigNum base = x % p;
	BigNum exp = y;

	while (exp > 0)
	{
		if ((exp.data[0] & 1) == 1)
		{ // If exp is odd
			res = (res * base) % p;
		}
		exp = exp >> 1;
		base = (base * base) % p;
	}
	return res;
}

bool RSA::millerTest(const BigNum &d, const BigNum &n)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	BigNum nMinusTwo = n - 2;

	std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());
	BigNum a(std::to_string(dis(gen) % (nMinusTwo.to_ulong() - 2) + 2));

	BigNum x = RSA::power(a, d, n);

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

BigNum RSA::generateRandomNumber(int bits)
{
	std::vector<u_char> bits_str = generateRandomBits(bits);
	BigNum number(bits_str);
	// Ensure the number is odd
	number.data[0] |= 1;
	return number;
}

std::vector<u_char> RSA::generateRandomBits(size_t length)
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
	return std::vector<u_char>(binaryString.begin(), binaryString.begin() + (length / 16));
}

BigNum RSA::generateLargePrime(int bits)
{
	BigNum prime;
	int count = 0;
	int k = 40; // Number of iterations for Miller-Rabin test
	do
	{
		prime = generateRandomNumber(bits);
	} while (!isPrime(prime, k));
	return prime;
}

BigNum RSA::gcd(BigNum a, BigNum b)
{
	while (b != 0)
	{
		BigNum t = b;
		b = a % b;
		a = t;
	}
	return a;
}

BigNum RSA::modInverse(BigNum a1, BigNum m1)
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

BigNum RSA::encrypt(const BigNum &message, const BigNum &publicKey, const BigNum &modulus)
{
	return power(message, publicKey, modulus);
}

BigNum RSA::decrypt(const BigNum &message, const BigNum &privateKey, const BigNum &modulus)
{
	return power(message, privateKey, modulus);
}

//

// std::vector<u_char> RSA::encrypt(const std::vector<u_char> &message, const BigNum &publicKey, const BigNum &modulus)
// {
//     int blockSize = modulus.getSizeThatIsFull() * BigNum::UINT_T_SIZE;
//     std::vector<u_char> result;

//     for (size_t i = 0; i < message.size(); i += blockSize / sizeof(char))
//     {
//         BigNum m(blockSize);
//         size_t blockLen = std::min(blockSize / sizeof(char), message.size() - i);

//         memcpy(m.data.data(), message.data() + i, blockLen * sizeof(char));

//         BigNum encryptedBlock = RSA::power(m, publicKey, modulus);
// 		std::vector<u_char> encryptedBlockStr = encryptedBlock.toString();
// 		while(encryptedBlockStr.size() < blockSize / 16){
// 			encryptedBlockStr = "0" + encryptedBlockStr;
// 		}
//         result += encryptedBlockStr;
//     }

//     return result;
// }

std::vector<u_char> RSA::encrypt(const std::vector<u_char> &message, const BigNum &publicKey, const BigNum &modulus)
{
	int blockSize = modulus.getSizeThatIsFull() - 1;
	int blockSizeInBits = blockSize * BigNum::UINT_T_SIZE;
	std::vector<u_char> result;
	int messageSizeInBit = message.size() * 8;

	for (int i = 0; i < messageSizeInBit; i += blockSizeInBits)
	{
		int blockLen = std::min(blockSizeInBits, messageSizeInBit - i);
		BigNum m(blockLen);

		memcpy(m.data.data(), message.data() + (i / 8), blockLen / 8);

		BigNum encryptedBlock = RSA::power(m, publicKey, modulus);
		std::vector<u_char> encryptedBlockStr = encryptedBlock.toString();

		result.emplace_back(encryptedBlockStr);
		result.emplace_back('P');
	}

	return result;
}

std::vector<u_char> RSA::encrypt(const std::vector<u_char> &message, const std::vector<u_char> &key)
{
	std::vector<u_char> modulus(key.begin(), key.begin() + key.find('P'));
	std::vector<u_char> publicKey(key.begin() + key.find('P') + 1, key.end());
	BigNum bigPublicKey(publicKey);
	BigNum bigModulus(modulus);
	return RSA::encrypt(message, bigPublicKey, bigModulus);
}

std::vector<u_char> RSA::decrypt(const std::vector<u_char> &encrypted_message, const BigNum &privateKey, const BigNum &modulus)
{
	std::vector<u_char> result;
	std::vector<u_char> encrypted_message_cpy(encrypted_message);
	while (encrypted_message_cpy.size() > 0)
	{
		BigNum encryptedBlock(std::vector<u_char>(encrypted_message_cpy.begin(), encrypted_message_cpy.begin() + encrypted_message_cpy.find('P')));
		encrypted_message_cpy = std::vector<u_char>(encrypted_message_cpy.begin() + encrypted_message_cpy.find('P') + 1, encrypted_message_cpy.end());
		BigNum decryptedBlock = RSA::power(encryptedBlock, privateKey, modulus);
		std::vector<u_char> decryptedBlockStr(decryptedBlock.getSizeThatIsFull() * BigNum::UINT_T_SIZE, '\0');
		memcpy(decryptedBlockStr.data(), decryptedBlock.data.data(), decryptedBlock.getSizeThatIsFull() * BigNum::UINT_T_SIZE);
		decryptedBlockStr.erase(std::remove(decryptedBlockStr.begin(), decryptedBlockStr.end(), '\0'), decryptedBlockStr.end());
		result.emplace_back(decryptedBlockStr);
	}

	return result;
}

std::vector<u_char> RSA::decrypt(const std::vector<u_char> &encrypted_message, const std::vector<u_char> &key)
{
	std::vector<u_char> modulus(key.begin(), key.begin() + key.find('P'));
	std::vector<u_char> privateKey(key.begin() + key.find('P') + 1, key.end());
	BigNum bigPrivateKey(privateKey);
	BigNum bigModulus(modulus);
	return RSA::decrypt(encrypted_message, bigPrivateKey, bigModulus);
}
