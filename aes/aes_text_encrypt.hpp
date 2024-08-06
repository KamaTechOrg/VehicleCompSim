#pragma once
#include "aes.hpp"

#if SYCL_ENABELED
#include <sycl/sycl.hpp>
#endif


namespace aes {


template <AesVariant Aes_var>
class AesTextEncrypt {
public:
    using KeyType = typename Aes<Aes_var>::KeyType;

    std::string static encrypt_ecb(KeyType const& key, std::string const& message);
    std::string static decrypt_ecb(KeyType const& key, std::string const& encrypted_message);

    std::string static encrypt_ecb(Aes<Aes_var> const& aes, std::string const& message);
    std::string static decrypt_ecb(Aes<Aes_var> const& aes, std::string const& encrypted_message);

#if SYCL_ENABELED
    std::string static encrypt_ecb(sycl::queue& q, KeyType const& key, std::string const& message);
    std::string static decrypt_ecb(sycl::queue& q, KeyType const& key, std::string const& message);

    std::string static encrypt_ecb(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& message);
    std::string static decrypt_ecb(sycl::queue& q, Aes<Aes_var> const& aes, std::string const& message);
#endif

    std::string static encrypt_cbc(KeyType const& key, std::string const& message, std::array<uint8_t, 16> const& iv);
    std::string static decrypt_cbc(KeyType const& key, std::string const& encrypted_message, std::array<uint8_t, 16> const& iv);

    std::string static encrypt_cbc(Aes<Aes_var> const& aes, std::string const& message, std::array<uint8_t, 16> const& iv);
    std::string static decrypt_cbc(Aes<Aes_var> const& aes, std::string const& encrypted_message, std::array<uint8_t, 16> const& iv);
};

} // namespace aes
