#include "RSA_ENC.h"
#include "RSA_KEY.h"
#include <random>
#include <iostream>
#include <string.h>

BigNum RSA_ENC::encrypt(const BigNum &message, const BigNum &publicKey, const BigNum &modulus)
{
	return RSA_KEY::power(message, publicKey, modulus);
}

BigNum RSA_ENC::decrypt(const BigNum &message, const BigNum &privateKey, const BigNum &modulus)
{
	return RSA_KEY::power(message, privateKey, modulus);
}

std::vector<u_int8_t> RSA_ENC::encrypt(const std::vector<u_int8_t> &message, const BigNum &publicKey, const BigNum &modulus)
{
	std::string messageStr(message.begin(), message.end());
	std::string result = encrypt(messageStr, publicKey, modulus);
	return std::vector<u_int8_t>(result.begin(), result.end());
}

std::string RSA_ENC::encrypt(const std::string &message, const BigNum &publicKey, const BigNum &modulus)
{
	int blockSize = modulus.getSizeThatIsFull() - 1;
	int blockSizeInBits = blockSize * BigNum::UINT_T_SIZE;
	std::string result;
	int messageSizeInBit = message.size() * 8;

	for (int i = 0; i < messageSizeInBit; i += blockSizeInBits)
	{
		int blockLen = std::min(blockSizeInBits, messageSizeInBit - i);
		BigNum m(blockLen * 2);

		memcpy(m.data.data(), message.data() + (i / 8), blockLen / 8);

		BigNum encryptedBlock = RSA_KEY::power(m, publicKey, modulus);
		std::string encryptedBlockStr = encryptedBlock.toString();

		result += encryptedBlockStr + 'P';
	}

	return result;
}

std::vector<u_int8_t> RSA_ENC::encrypt(const std::vector<u_int8_t> &message, const std::vector<u_int8_t> &key)
{
	std::vector<u_int8_t> modulus(key.begin(), std::find(key.begin(), key.end(), 'P'));
	std::vector<u_int8_t> publicKey(key.begin() + modulus.size() + 1, key.end());
	BigNum bigPublicKey(publicKey);
	BigNum bigModulus(modulus);
	return RSA_ENC::encrypt(message, bigPublicKey, bigModulus);
}

std::vector<u_int8_t> RSA_ENC::decrypt(const std::vector<u_int8_t> &encrypted_message, const BigNum &privateKey, const BigNum &modulus)
{
	std::string encrypted_message_str(encrypted_message.begin(), encrypted_message.end());
	std::string result = decrypt(encrypted_message_str, privateKey, modulus);
	return std::vector<u_int8_t>(result.begin(), result.end());
}

std::string RSA_ENC::decrypt(const std::string &encrypted_message, const BigNum &privateKey, const BigNum &modulus)
{
	std::string result;
	std::string encrypted_message_cpy(encrypted_message);
	while (encrypted_message_cpy.size() > 0)
	{
		BigNum encryptedBlock(encrypted_message_cpy.substr(0, encrypted_message_cpy.find('P')));
		encrypted_message_cpy = encrypted_message_cpy.substr(encrypted_message_cpy.find('P') + 1);
		BigNum decryptedBlock = RSA_KEY::power(encryptedBlock, privateKey, modulus);
		std::string decryptedBlockStr(decryptedBlock.getSizeThatIsFull() * BigNum::UINT_T_SIZE, '\0');
		memcpy(decryptedBlockStr.data(), decryptedBlock.data.data(), decryptedBlock.getSizeThatIsFull() * BigNum::UINT_T_SIZE);
		decryptedBlockStr.erase(decryptedBlockStr.find_last_not_of('\0') + 1);
		result += decryptedBlockStr;
	}

	return result;
}

std::vector<u_int8_t> RSA_ENC::decrypt(const std::vector<u_int8_t> &encrypted_message, const std::vector<u_int8_t> &key)
{
	std::vector<u_int8_t> modulus(key.begin(), std::find(key.begin(), key.end(), 'P'));
	std::vector<u_int8_t> privateKey(key.begin() + modulus.size() + 1, key.end());
	BigNum bigPrivateKey(privateKey);
	BigNum bigModulus(modulus);
	return RSA_ENC::decrypt(encrypted_message, bigPrivateKey, bigModulus);
}
