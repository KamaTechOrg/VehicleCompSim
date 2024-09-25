#include <memory>
#include <stdexcept>

#include "AES_API.hpp"
#include "AES_chaining_stream.hpp"
#include "AES_block_encrypt.hpp"

#include "../../HsmStatus/HSMStatusEnum.hpp"

using namespace HSM;
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

HSM_STATUS expected_key_size_and_type_check(std::string const& type, uint& key_size) {
  if(type.compare(0, 4, "AES-") != 0) 
      return HSM_STATUS::HSM_InvalidAlg;
  uint iv_size = type.compare(8, 3, "CBC") ? 16 
              :( type.compare(8, 3, "CTR") ? 12
              :  type.compare(8, 3, "ECB") ? 0
              : /* error */ 1000);
  uint aes_key_size = type.compare(4, 3, "128") ? 16 
                :(type.compare(4, 3, "192") ? 24 
                : type.compare(4, 3, "256") ? 32 
                : /* error */ 1000);
  
  if(iv_size + aes_key_size >= 1000)
    return HSM_STATUS::HSM_InvalidAlg;
  
  key_size = iv_size + aes_key_size;
  return HSM_STATUS::HSM_Good;
}


HSM_STATUS AES::encrypt(std::vector<uint8_t> const &key, std::vector<uint8_t> const &clrMsg, std::vector<uint8_t> &encMsg, std::string const &type) {
    uint expected_key_size;
    if(auto stat = expected_key_size_and_type_check(type, expected_key_size); stat != HSM_STATUS::HSM_Good)
      throw stat;
    
    if(key.size() != expected_key_size)
      throw HSM::HSM_KeyNotValid;
    auto aes = aes_create(key, type);

    if(type.compare(8, 3, "ECB") == 0) {
        encMsg = AES_ChainingStream::encrypt_ecb(*aes, clrMsg);
    } else if(type.compare(8, 3, "CBC") == 0) {
        std::array<uint8_t, 16> iv;
        std::array<uint8_t, 16> key_arr;
        std::copy(key.end()-16, key.end(), iv.begin());
        encMsg = AES_ChainingStream::encrypt_cbc(*aes, iv, clrMsg);
    } else if (type.compare(8, 3, "CTR") == 0) {
        std::array<uint8_t, 16> key_arr;
        std::array<uint8_t, 8> iv;
        std::array<uint8_t, 4> nonce;
        std::copy(key.end()-12, key.end()-4, iv.begin());
        std::copy(key.end()-4, key.end(), nonce.begin());
        encMsg = AES_ChainingStream::encrypt_ctr(*aes,  iv, nonce, clrMsg);
    } else {
        throw std::runtime_error{"We are not supposed to get here"};
    }
    return HSM_STATUS::HSM_Good;
}


HSM_STATUS AES::decrypt(std::vector<uint8_t> const &key, std::vector<uint8_t> const &encMsg, std::vector<uint8_t> &clrMsg, std::string const &type) {
    uint expected_key_size;
    if(auto stat = expected_key_size_and_type_check(type, expected_key_size); stat != HSM_STATUS::HSM_Good)
      return stat;

    if(key.size() != expected_key_size)
      throw HSM::HSM_KeyNotValid;
    auto aes = aes_create(key, type);

    try {

        if(type.compare(8, 3, "ECB") == 0) {
            clrMsg = AES_ChainingStream::decrypt_ecb(*aes, encMsg);
        } else if(type.compare(8, 3, "CBC") == 0) {
            std::array<uint8_t, 16> iv;
            std::array<uint8_t, 16> key_arr;
            std::copy(key.end()-16, key.end(), iv.begin());
            clrMsg = AES_ChainingStream::decrypt_cbc(*aes, iv, encMsg);
        } else /* if (type.compare(8, 3, "CTR") == 0) */{ 
            std::array<uint8_t, 16> key_arr;
            std::array<uint8_t, 8> iv;
            std::array<uint8_t, 4> nonce;
            std::copy(key.end()-12, key.end()-4, iv.begin());
            std::copy(key.end()-4, key.end(), nonce.begin());
            clrMsg = AES_ChainingStream::encrypt_ctr(*aes,  iv, nonce, encMsg);
        }
    } catch (std::length_error const&) {
        return HSM_STATUS::HSM_WrongDataSize;
    } catch (std::invalid_argument const&) {
        return HSM_STATUS::HSM_WrongPadding;
    }
    return HSM_STATUS::HSM_Good;
}


HSM_STATUS AES::generateKey( std::vector<uint8_t>& generated_key, std::string const &type)
{
    uint key_size;
    if(auto status = expected_key_size_and_type_check(type, key_size); status != HSM_Good)
        return status;

    generated_key.resize(key_size);
    HSM_STATUS ret = HSM_STATUS::HSM_InternalErr;
#ifdef __linux__
    auto fd = fopen("/dev/random", "r");
    if(!fd)
        return ret;
    if(fread(generated_key.data(), 1, generated_key.size(), fd) == key_size)
        ret = HSM_STATUS::HSM_Good;
    fclose(fd);
#endif
    return ret;
}
