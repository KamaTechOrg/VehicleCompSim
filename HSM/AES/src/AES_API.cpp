#include <memory>
#include <stdexcept>

#include "AES_API.hpp"
#include "AES_chaining_stream.hpp"
#include "AES_block_encrypt.hpp"

using namespace aes;

std::unique_ptr<AES_block_encrypt> aes_create(std::vector<uint8_t> const& key, std::string const& type) {
  if(type.compare(4, 3, "128") == 0) {
    std::array<uint8_t, 16> key_arr;
    std::copy(key.begin(), key.begin()+16, key_arr.begin());
    return std::make_unique<AES_block_encrypt_128>(key_arr);
  } else if (type.compare(4, 3, "192") == 0) {
    std::array<uint8_t, 24> key_arr;
    std::copy(key.begin(), key.begin()+24, key_arr.begin());
    return std::make_unique<AES_block_encrypt_192>(key_arr);
  } else if (type.compare(4, 3, "256") == 0) {
    std::array<uint8_t, 32> key_arr;
    std::copy(key.begin(), key.begin()+32, key_arr.begin());
    return std::make_unique<AES_block_encrypt_256>(key_arr);
  } else {
    throw std::invalid_argument{"invalid type :" + type};
  }
}

uint expected_key_size(std::string const& type) {
  uint iv_size = type.compare(8, 3, "CBC") ? 16 : (
                 type.compare(8, 3, "CTR") ? 12 : /*ECB*/ 0 );
  uint key_size = type.compare(4, 3, "128") ? 16 : (type.compare(4, 3, "192") ? 24 : 32);
  return iv_size + key_size;
}


std::vector<uint8_t> AES::encrypt(const std::vector<uint8_t> &key, const std::vector<uint8_t> &message, const std::string &type) {
    if(type.compare(0, 4, "AES-") != 0) 
        throw std::invalid_argument{"invalid type :" + type};
    if(key.size() != expected_key_size(type))
      throw std::invalid_argument{"invalid key for " + type};
    auto aes = aes_create(key, type);

    if(type.compare(8, 3, "ECB") == 0) {
        return AES_ChainingStream::encrypt_ecb(*aes, message);
    } else if(type.compare(8, 3, "CBC") == 0) {
        std::array<uint8_t, 16> iv;
        std::array<uint8_t, 16> key_arr;
        std::copy(key.end()-16, key.end(), iv.begin());
        return AES_ChainingStream::encrypt_cbc(*aes, iv, message);
    } else if (type.compare(8, 3, "CTR") == 0) {
        std::array<uint8_t, 16> key_arr;
        std::array<uint8_t, 8> iv;
        std::array<uint8_t, 4> nonce;
        std::copy(key.end()-12, key.end()-4, iv.begin());
        std::copy(key.end()-4, key.end(), nonce.begin());
        return AES_ChainingStream::encrypt_ctr(*aes,  iv, nonce, message);
    } else {
        throw std::invalid_argument{"invalid type :" + type};
    }
}


std::vector<uint8_t> AES::decrypt(const std::vector<uint8_t> &key, const std::vector<uint8_t> &message, const std::string &type) {
    if(type.compare(0, 4, "AES-") != 0) 
        throw std::invalid_argument{"invalid type :" + type};
    if(key.size() != expected_key_size(type))
        throw std::invalid_argument{"invalid key for " + type};
    auto aes = aes_create(key, type);

    if(type.compare(8, 3, "ECB") == 0) {
        return AES_ChainingStream::decrypt_ecb(*aes, message);
    } else if(type.compare(8, 3, "CBC") == 0) {
        std::array<uint8_t, 16> iv;
        std::array<uint8_t, 16> key_arr;
        std::copy(key.end()-16, key.end(), iv.begin());
        return AES_ChainingStream::decrypt_cbc(*aes, iv, message);
    } else if (type.compare(8, 3, "CTR") == 0) {
        std::array<uint8_t, 16> key_arr;
        std::array<uint8_t, 8> iv;
        std::array<uint8_t, 4> nonce;
        std::copy(key.end()-12, key.end()-4, iv.begin());
        std::copy(key.end()-4, key.end(), nonce.begin());
        return AES_ChainingStream::encrypt_ctr(*aes,  iv, nonce, message);
    } else {
        throw std::invalid_argument{"invalid type :" + type};
    }
}
