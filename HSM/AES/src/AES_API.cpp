#include "AES_API.hpp"
#include "aes_text_encrypt.hpp"
#include "aes.hpp"
using namespace aes;

// 🤮🤮🤮🤮🤮🤮
std::string AES::encrypt(const std::string &type, const std::string &key, const std::string &message) {
    if(type.compare(0, 4, "AES-") != 0) 
        throw std::invalid_argument{"invalid type :" + type};
    if(type.compare(8, 3, "ECB") == 0) {
        if(type.compare(4, 3, "128") == 0) {
          if(key.size() != 16) throw std::invalid_argument{""};
          std::array<uint8_t, 16> key_arr;
          std::copy(key.begin(), key.end(), key_arr.begin());
          return AesTextEncrypt<AesVariant::Aes128>::encrypt_ecb(key_arr, message);
        } else if (type.compare(4, 3, "192") == 0) {
          if(key.size() != 24) throw std::invalid_argument{""};
          std::array<uint8_t, 24> key_arr;
          std::copy(key.begin(), key.end(), key_arr.begin());
          return AesTextEncrypt<AesVariant::Aes192>::encrypt_ecb(key_arr, message);
        } else if (type.compare(4, 3, "256") == 0) {
          if(key.size() != 32) throw std::invalid_argument{""};
          std::array<uint8_t, 32> key_arr;
          std::copy(key.begin(), key.end(), key_arr.begin());
          return AesTextEncrypt<AesVariant::Aes256>::encrypt_ecb(key_arr, message);
        } else {
            throw std::invalid_argument{"invalid type :" + type};
        }
    } else if(type.compare(8, 3, "CBC") == 0) {
        if(type.compare(4, 3, "128") == 0) {
          if(key.size() != 32) throw std::invalid_argument{""};
          std::array<uint8_t, 16> iv;
          std::array<uint8_t, 16> key_arr;
          std::copy(key.begin(), key.begin()+16, key_arr.begin());
          std::copy(key.begin()+16, key.end(), iv.begin());
          return AesTextEncrypt<AesVariant::Aes128>::encrypt_cbc(key_arr, message, iv);
        } else if (type.compare(4, 3, "192") == 0) {
          if(key.size() != 40) throw std::invalid_argument{""};
          std::array<uint8_t, 16> iv;
          std::array<uint8_t, 24> key_arr;
          std::copy(key.begin(), key.begin()+24, key_arr.begin());
          std::copy(key.begin()+24, key.end(), iv.begin());
          return AesTextEncrypt<AesVariant::Aes192>::encrypt_cbc(key_arr, message, iv);        
        } else if (type.compare(4, 3, "256") == 0) {
          if(key.size() != 48) throw std::invalid_argument{""};
          std::array<uint8_t, 16> iv;
          std::array<uint8_t, 32> key_arr;
          std::copy(key.begin(), key.begin()+32, key_arr.begin());
          std::copy(key.begin()+32, key.end(), iv.begin());
          return AesTextEncrypt<AesVariant::Aes256>::encrypt_cbc(key_arr, message, iv);
        } else {
            throw std::invalid_argument{"invalid type :" + type};
        }
    } else if (type.compare(8, 3, "CTR") == 0) {
        if(type.compare(4, 3, "128") == 0) {
          if(key.size() != 28) throw std::invalid_argument{""};
          std::array<uint8_t, 16> key_arr;
          std::array<uint8_t, 8> iv;
          std::array<uint8_t, 4> nonce;
          std::copy(key.begin(), key.begin()+16, key_arr.begin());
          std::copy(key.begin()+16, key.begin()+24, iv.begin());
          std::copy(key.begin()+24, key.end(), nonce.begin());
          return AesTextEncrypt<AesVariant::Aes128>::encrypt_ctr(key_arr, message, iv, nonce);
        } else if (type.compare(4, 3, "192") == 0) {
          if(key.size() != 36) throw std::invalid_argument{""};
          std::array<uint8_t, 24> key_arr;
          std::array<uint8_t, 8> iv;
          std::array<uint8_t, 4> nonce;
          std::copy(key.begin(), key.begin()+24, key_arr.begin());
          std::copy(key.begin()+16, key.begin()+32, iv.begin());
          std::copy(key.begin()+32, key.end(), nonce.begin());
          return AesTextEncrypt<AesVariant::Aes192>::encrypt_ctr(key_arr, message, iv, nonce);
        } else if (type.compare(4, 3, "256") == 0) {
          if(key.size() != 44) throw std::invalid_argument{""};
          std::array<uint8_t, 32> key_arr;
          std::array<uint8_t, 8> iv;
          std::array<uint8_t, 4> nonce;
          std::copy(key.begin(), key.begin()+32, key_arr.begin());
          std::copy(key.begin()+32, key.begin()+40, iv.begin());
          std::copy(key.begin()+40, key.end(), nonce.begin());
          return AesTextEncrypt<AesVariant::Aes256>::encrypt_ctr(key_arr, message, iv, nonce);
        } else {
            throw std::invalid_argument{"invalid type :" + type};
        }
    } else {
        throw std::invalid_argument{"invalid type :" + type};
    }
}


std::string AES::decrypt(const std::string &type, const std::string &key, const std::string &message) {
    if(type.compare(0, 4, "AES-") != 0) 
        throw std::invalid_argument{"invalid type :" + type};
    if(type.compare(8, 3, "ECB") == 0) {
        if(type.compare(4, 3, "128") == 0) {
          if(key.size() != 16) throw std::invalid_argument{""};
          std::array<uint8_t, 16> key_arr;
          std::copy(key.begin(), key.end(), key_arr.begin());
          return AesTextEncrypt<AesVariant::Aes128>::decrypt_ecb(key_arr, message);
        } else if (type.compare(4, 3, "192") == 0) {
          if(key.size() != 24) throw std::invalid_argument{""};
          std::array<uint8_t, 24> key_arr;
          std::copy(key.begin(), key.end(), key_arr.begin());
          return AesTextEncrypt<AesVariant::Aes192>::decrypt_ecb(key_arr, message);
        } else if (type.compare(4, 3, "256") == 0) {
          if(key.size() != 32) throw std::invalid_argument{""};
          std::array<uint8_t, 32> key_arr;
          std::copy(key.begin(), key.end(), key_arr.begin());
          return AesTextEncrypt<AesVariant::Aes256>::decrypt_ecb(key_arr, message);
        } else {
            throw std::invalid_argument{"invalid type :" + type};
        }
    } else if(type.compare(8, 3, "CBC") == 0) {
        if(type.compare(4, 3, "128") == 0) {
          if(key.size() != 32) throw std::invalid_argument{""};
          std::array<uint8_t, 16> iv;
          std::array<uint8_t, 16> key_arr;
          std::copy(key.begin(), key.begin()+16, key_arr.begin());
          std::copy(key.begin()+16, key.end(), iv.begin());
          return AesTextEncrypt<AesVariant::Aes128>::decrypt_cbc(key_arr, message, iv);
        } else if (type.compare(4, 3, "192") == 0) {
          if(key.size() != 40) throw std::invalid_argument{""};
          std::array<uint8_t, 16> iv;
          std::array<uint8_t, 24> key_arr;
          std::copy(key.begin(), key.begin()+24, key_arr.begin());
          std::copy(key.begin()+24, key.end(), iv.begin());
          return AesTextEncrypt<AesVariant::Aes192>::decrypt_cbc(key_arr, message, iv);        
        } else if (type.compare(4, 3, "256") == 0) {
          if(key.size() != 48) throw std::invalid_argument{""};
          std::array<uint8_t, 16> iv;
          std::array<uint8_t, 32> key_arr;
          std::copy(key.begin(), key.begin()+32, key_arr.begin());
          std::copy(key.begin()+32, key.end(), iv.begin());
          return AesTextEncrypt<AesVariant::Aes256>::decrypt_cbc(key_arr, message, iv);
        } else {
            throw std::invalid_argument{"invalid type :" + type};
        }
    } else if (type.compare(8, 3, "CTR") == 0) {
        if(type.compare(4, 3, "128") == 0) {
          if(key.size() != 28) throw std::invalid_argument{""};
          std::array<uint8_t, 16> key_arr;
          std::array<uint8_t, 8> iv;
          std::array<uint8_t, 4> nonce;
          std::copy(key.begin(), key.begin()+16, key_arr.begin());
          std::copy(key.begin()+16, key.begin()+24, iv.begin());
          std::copy(key.begin()+24, key.end(), nonce.begin());
          return AesTextEncrypt<AesVariant::Aes128>::encrypt_ctr(key_arr, message, iv, nonce);
        } else if (type.compare(4, 3, "192") == 0) {
          if(key.size() != 36) throw std::invalid_argument{""};
          std::array<uint8_t, 24> key_arr;
          std::array<uint8_t, 8> iv;
          std::array<uint8_t, 4> nonce;
          std::copy(key.begin(), key.begin()+24, key_arr.begin());
          std::copy(key.begin()+16, key.begin()+32, iv.begin());
          std::copy(key.begin()+32, key.end(), nonce.begin());
          return AesTextEncrypt<AesVariant::Aes192>::encrypt_ctr(key_arr, message, iv, nonce);
        } else if (type.compare(4, 3, "256") == 0) {
          if(key.size() != 44) throw std::invalid_argument{""};
          std::array<uint8_t, 32> key_arr;
          std::array<uint8_t, 8> iv;
          std::array<uint8_t, 4> nonce;
          std::copy(key.begin(), key.begin()+32, key_arr.begin());
          std::copy(key.begin()+32, key.begin()+40, iv.begin());
          std::copy(key.begin()+40, key.end(), nonce.begin());
          return AesTextEncrypt<AesVariant::Aes256>::encrypt_ctr(key_arr, message, iv, nonce);
        } else {
            throw std::invalid_argument{"invalid type :" + type};
        }
    } else {
        throw std::invalid_argument{"invalid type :" + type};
    }
}


