#pragma once
#include <vector>
#include "AES_block_encrypt.hpp"

#if SYCL_ENABLED
#include <sycl/sycl.hpp>
#endif


namespace aes {


// template <AesVariant Aes_var>
class AES_ChainingStream {
public:

// ecb mode
    std::vector<uint8_t> static encrypt_ecb(AES_block_encrypt const& aes, std::vector<uint8_t> const& message);
    std::vector<uint8_t> static decrypt_ecb(AES_block_encrypt const& aes, std::vector<uint8_t> const& encrypted_message);

// cbc mode
#if SYCL_ENABLED
    std::vector<uint8_t> static encrypt_ecb(sycl::queue& q, AES_block_encrypt const& aes, std::vector<uint8_t> const& message);
    std::vector<uint8_t> static decrypt_ecb(sycl::queue& q, AES_block_encrypt const& aes, std::vector<uint8_t> const& encrypted_message);
#endif

    std::vector<uint8_t> static encrypt_cbc(AES_block_encrypt const& aes, std::array<uint8_t, 16> const& iv, std::vector<uint8_t> const& message);
    std::vector<uint8_t> static decrypt_cbc(AES_block_encrypt const& aes, std::array<uint8_t, 16> const& iv,  std::vector<uint8_t> const& encrypted_message);

// ctr mode
// note: in ctr mode decrypt and encrypt are the same
    std::vector<uint8_t> static encrypt_ctr(AES_block_encrypt const& aes, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce, std::vector<uint8_t> const& message);

#if SYCL_ENABLED
    std::vector<uint8_t> static encrypt_ctr(sycl::queue& q, AES_block_encrypt const& aes, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce, std::vector<uint8_t> const& message);
#endif

};

} // namespace aes
