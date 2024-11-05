#pragma once
#include "aes.hpp"

#if SYCL_ENABLED
#include <sycl/sycl.hpp>
#endif


namespace aes {


template <AesVariant Aes_var>
class AesTextEncrypt {
public:
    using KeyType = typename Aes<Aes_var>::KeyType;

// ecb mode
    std::string static encrypt_ecb(KeyType const& key, std::string const& message);
    std::string static decrypt_ecb(KeyType const& key, std::string const& encrypted_message);

    std::string static encrypt_ecb(Aes<Aes_var> const& aes, std::string const& message);
    std::string static encrypt_ecb_par(Aes<Aes_var> const& aes, std::string const& message);
    std::string static decrypt_ecb(Aes<Aes_var> const& aes, std::string const& encrypted_message);




// cbc mode
#if SYCL_ENABLED
    std::string static encrypt_ecb(sycl::queue& q, KeyType const& key, std::string const& message);
    std::string static decrypt_ecb(sycl::queue& q, KeyType const& key, std::string const& message);

    std::string static encrypt_ecb(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& message);
    std::string static decrypt_ecb(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& message);
#endif

    std::string static encrypt_cbc(Aes<Aes_var> const& aes, std::string const& message, std::array<uint8_t, 16> const& iv);
    std::string static decrypt_cbc(Aes<Aes_var> const& aes, std::string const& encrypted_message, std::array<uint8_t, 16> const& iv);
    
    std::string static encrypt_cbc(KeyType const& key, std::string const& message, std::array<uint8_t, 16> const& iv);
    std::string static decrypt_cbc(KeyType const& key, std::string const& encrypted_message, std::array<uint8_t, 16> const& iv);

// ctr mode
// note: in ctr mode decrypt and encrypt are the same
    std::string static encrypt_ctr(KeyType const& key, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce);
    std::string static encrypt_ctr(Aes<Aes_var> const& aes, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce);

#if SYCL_ENABLED
    std::string static encrypt_ctr(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce);
    std::string static encrypt_ctr(sycl::queue& q, KeyType const& key, std::string const& message, std::array<uint8_t, 8> const& iv, std::array<uint8_t, 4> const& nonce);
#endif

};

} // namespace aes
