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

std::vector<u_char> RSA_ENC::encrypt(const std::vector<u_char> &message, const BigNum &publicKey, const BigNum &modulus)
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

		BigNum encryptedBlock = RSA_KEY::power(m, publicKey, modulus);
		std::vector<u_char> encryptedBlockStr = encryptedBlock.toString();

		result.emplace_back(encryptedBlockStr);
		result.emplace_back('P');
	}

	return result;
}

std::vector<u_char> RSA_ENC::encrypt(const std::vector<u_char> &message, const std::vector<u_char> &key)
{
	std::vector<u_char> modulus(key.begin(), key.begin() + key.find('P'));
	std::vector<u_char> publicKey(key.begin() + key.find('P') + 1, key.end());
	BigNum bigPublicKey(publicKey);
	BigNum bigModulus(modulus);
	return RSA_ENC::encrypt(message, bigPublicKey, bigModulus);
}

std::vector<u_char> RSA_ENC::decrypt(const std::vector<u_char> &encrypted_message, const BigNum &privateKey, const BigNum &modulus)
{
	std::vector<u_char> result;
	std::vector<u_char> encrypted_message_cpy(encrypted_message);
	while (encrypted_message_cpy.size() > 0)
	{
		BigNum encryptedBlock(std::vector<u_char>(encrypted_message_cpy.begin(), encrypted_message_cpy.begin() + encrypted_message_cpy.find('P')));
		encrypted_message_cpy = std::vector<u_char>(encrypted_message_cpy.begin() + encrypted_message_cpy.find('P') + 1, encrypted_message_cpy.end());
		BigNum decryptedBlock = RSA_KEY::power(encryptedBlock, privateKey, modulus);
		std::vector<u_char> decryptedBlockStr(decryptedBlock.getSizeThatIsFull() * BigNum::UINT_T_SIZE, '\0');
		memcpy(decryptedBlockStr.data(), decryptedBlock.data.data(), decryptedBlock.getSizeThatIsFull() * BigNum::UINT_T_SIZE);
		decryptedBlockStr.erase(std::remove(decryptedBlockStr.begin(), decryptedBlockStr.end(), '\0'), decryptedBlockStr.end());
		result.emplace_back(decryptedBlockStr);
	}

	return result;
}

std::vector<u_char> RSA_ENC::decrypt(const std::vector<u_char> &encrypted_message, const std::vector<u_char> &key)
{
	std::vector<u_char> modulus(key.begin(), key.begin() + key.find('P'));
	std::vector<u_char> privateKey(key.begin() + key.find('P') + 1, key.end());
	BigNum bigPrivateKey(privateKey);
	BigNum bigModulus(modulus);
	return RSA_ENC::decrypt(encrypted_message, bigPrivateKey, bigModulus);
}
