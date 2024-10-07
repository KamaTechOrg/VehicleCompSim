#include <memory>
#include <fstream>
#include <stdexcept>
#include <iomanip>

#include "AES_API.hpp"
#include "AES_chaining_stream.hpp"
#include "AES_block_encrypt.hpp"

#include "../../HSM_Enums/HSMEnums.hpp"

using namespace HSM;
using namespace aes;

bool is_AES_algo_type(ENCRYPTION_ALGORITHM_TYPE const& type){
  return type >= ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB && type <= ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR;
}

AesVariant AES_variant(ENCRYPTION_ALGORITHM_TYPE const& type){
  switch (type)
  {
  case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB:
  case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC:
  case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR:
    return AesVariant::Aes128;
  
  case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB:
  case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC:
  case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR:
    return AesVariant::Aes192;

  case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB:
  case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC:
  case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR:
    return AesVariant::Aes256;  
  default:
    return AesVariant::NoAesVariant;
  }
}



std::unique_ptr<AES_block_encrypt> aes_create(std::vector<uint8_t> const& key, ENCRYPTION_ALGORITHM_TYPE const& type) {
  switch (type)
  {
  case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB:
  case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC:
  case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR: {
    std::array<uint8_t, 16> key_arr;
    std::copy(key.begin(), key.begin()+16, key_arr.begin());
    return std::make_unique<AES_block_encrypt_128>(key_arr);
  }

  case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB:
  case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC:
  case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR: {
    std::array<uint8_t, 24> key_arr;
    std::copy(key.begin(), key.begin()+24, key_arr.begin());
    return std::make_unique<AES_block_encrypt_192>(key_arr);
  }

  case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB:
  case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC:
  case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR: {
    std::array<uint8_t, 32> key_arr;
    std::copy(key.begin(), key.begin()+32, key_arr.begin());
    return std::make_unique<AES_block_encrypt_256>(key_arr);
  }

  default:
    throw std::invalid_argument{"invalid AES type:" + type};
  }
}



HSM_STATUS expected_key_size_and_type_check(ENCRYPTION_ALGORITHM_TYPE const& type, uint& key_size) {
  switch (type)
  {
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB: key_size = 16; break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC: key_size = 32; break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR: key_size = 28; break; 
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB: key_size = 24; break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC: key_size = 40; break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR: key_size = 36; break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB: key_size = 32; break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC: key_size = 48; break;
    case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR: key_size = 44; break;
    default:
      return HSM_STATUS::HSM_InvalidAlg;
  }
  return HSM_STATUS::HSM_Good;
}


HSM_STATUS AES::encrypt(std::vector<uint8_t> const &clrMsg, std::vector<uint8_t> &encMsg, std::vector<uint8_t> const &key, ENCRYPTION_ALGORITHM_TYPE const &type) {
    uint expected_key_size;
    if(auto stat = expected_key_size_and_type_check(type, expected_key_size); stat != HSM_STATUS::HSM_Good)
      return stat;
    
    if(key.size() != expected_key_size)
      return HSM::HSM_KeyNotValid;
    auto aes = aes_create(key, type);

    switch (type)
    {
        case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB:
        case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB:
        case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB:
            encMsg = AES_ChainingStream::encrypt_ecb(*aes, clrMsg);
            break;

        case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC:
        case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC:
        case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC: {
            std::array<uint8_t, 16> iv;
            std::copy(key.end()-16, key.end(), iv.begin());
            encMsg = AES_ChainingStream::encrypt_cbc(*aes, iv, clrMsg);
        } break;

        case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR:
        case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR:
        case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR: {
            std::array<uint8_t, 8> iv;
            std::array<uint8_t, 4> nonce;
            std::copy(key.end()-12, key.end()-4, iv.begin());
            std::copy(key.end()-4, key.end(), nonce.begin());
            encMsg = AES_ChainingStream::encrypt_ctr(*aes,  iv, nonce, clrMsg);
        } break;

        default:
            return HSM_STATUS::HSM_InternalErr; // This shouldn't happen
    }
    return HSM_STATUS::HSM_Good;
}



HSM_STATUS AES::decrypt(std::vector<uint8_t> const &clrMsg, std::vector<uint8_t> &encMsg, std::vector<uint8_t> const &key, ENCRYPTION_ALGORITHM_TYPE const &type) {
    uint expected_key_size;
    if(auto stat = expected_key_size_and_type_check(type, expected_key_size); stat != HSM_STATUS::HSM_Good)
      return stat;
    
    if(key.size() != expected_key_size)
      return HSM::HSM_KeyNotValid;
    auto aes = aes_create(key, type);

    switch (type)
    {
        case ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB:
        case ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB:
        case ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB:
            encMsg = AES_ChainingStream::decrypt_ecb(*aes, clrMsg);
            break;

        case ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC:
        case ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC:
        case ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC: {
            std::array<uint8_t, 16> iv;
            std::copy(key.end()-16, key.end(), iv.begin());
            encMsg = AES_ChainingStream::decrypt_cbc(*aes, iv, clrMsg);
        } break;

        case ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR:
        case ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR:
        case ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR: {
            std::array<uint8_t, 8> iv;
            std::array<uint8_t, 4> nonce;
            std::copy(key.end()-12, key.end()-4, iv.begin());
            std::copy(key.end()-4, key.end(), nonce.begin());
            encMsg = AES_ChainingStream::encrypt_ctr(*aes,  iv, nonce, clrMsg);
        } break;

        default:
            return HSM_STATUS::HSM_InternalErr; // This shouldn't happen
    }
    return HSM_STATUS::HSM_Good;
}


HSM_STATUS AES::generateKey( std::vector<uint8_t>& generated_key, ENCRYPTION_ALGORITHM_TYPE const &type)
{
    uint key_size;
    if(auto status = expected_key_size_and_type_check(type, key_size); status != HSM_Good)
        return status;
    try {
        generated_key.resize(key_size);
// #ifdef __linux__
        std::ifstream ifs("/dev/random");
        ifs.read(reinterpret_cast<char*>(generated_key.data()), generated_key.size());
        if(ifs.good())
            return HSM_STATUS::HSM_Good;
// #endif
    } catch (std::exception const&) {}
    return HSM_STATUS::HSM_InternalErr;
}


HSM_STATUS AES::generateAndPrintKey( std::vector<uint8_t>& generated_key, ENCRYPTION_ALGORITHM_TYPE const &type)
{
  HSM_STATUS status = generateKey(generated_key, type);
  if(status != HSM_STATUS::HSM_Good)
    return status;
  std::stringstream ss;
  for(auto const& b : generated_key)
    ss << std::hex << std::setfill('0') << std::setw(2) << (int)b;
  std::cout << ss.str() << std::dec << std::endl;
  return status;
  // for(auto const& b : generated_key)
  //   std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)b;
  // std::cout << std::dec << std::endl;
  // return status;
}
