#include <benchmark/benchmark.h>
#include <cstddef>
#include <numeric>
#include <array>

#include "aes.hpp"
#include "aes_text_encrypt.hpp"

using namespace aes;

template<AesVariant Aes_var>
static const typename Aes<Aes_var>::KeyType aes_key;

template<>
const Aes<AesVariant::Aes128>::KeyType aes_key<AesVariant::Aes128> = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};

template<>
const Aes<AesVariant::Aes192>::KeyType aes_key<AesVariant::Aes192> = {
      0xbb, 0x85, 0x57, 0x2b, 0x2b, 0x82, 0xce, 0xc0, 0x93, 0x3b, 0xa9,
      0x38, 0x55, 0x77, 0x9b, 0xd3, 0xd4, 0xca, 0xd5, 0x93, 0x5f, 0xc0,
      0x84, 0x85};


template<>
const Aes<AesVariant::Aes256>::KeyType aes_key<AesVariant::Aes256> = {
      0xbb, 0x85, 0x57, 0x2b, 0x2b, 0x82, 0xce, 0xc0, 0x93, 0x3b, 0xa9,
      0x38, 0x55, 0x77, 0x9b, 0xd3, 0xd4, 0xca, 0xd5, 0x93, 0x5f, 0xc0,
      0x84, 0x85, 0x8d, 0xc3, 0x71, 0xb0, 0x65, 0x4a, 0x60, 0x31};


std::array<uint8_t, 16> iv = {0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30, 0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41};


static const std::string msg4k = []() -> std::string {
    std::string message(1<<12, '\0');
    std::iota(message.begin(), message.end(), 1);
    return message;
}();

template<AesVariant Aes_var>
static void msg4k_encrypt_aes_ecb(benchmark::State& state) {
    Aes<Aes_var> aes(aes_key<Aes_var>);
    for (auto _ : state){
        AesTextEncrypt<Aes_var>::encrypt_ecb(aes, msg4k);
    }
}
BENCHMARK(msg4k_encrypt_aes_ecb<AesVariant::Aes128>);
BENCHMARK(msg4k_encrypt_aes_ecb<AesVariant::Aes192>);
BENCHMARK(msg4k_encrypt_aes_ecb<AesVariant::Aes256>);

template<AesVariant Aes_var>
static void msg4k_decrypt_aes_ecb(benchmark::State& state) {
    Aes<Aes_var> aes(aes_key<Aes_var>);
    auto decrypted_msg = AesTextEncrypt<Aes_var>::encrypt_ecb(aes, msg4k);
    for (auto _ : state){
        AesTextEncrypt<Aes_var>::decrypt_ecb(aes, decrypted_msg);
    }
}
BENCHMARK(msg4k_decrypt_aes_ecb<AesVariant::Aes128>);
BENCHMARK(msg4k_decrypt_aes_ecb<AesVariant::Aes192>);
BENCHMARK(msg4k_decrypt_aes_ecb<AesVariant::Aes256>);

#if SYCL_ENABELED
#include <sycl/sycl.hpp>
sycl::queue q;

template<AesVariant Aes_var>
static void msg4k_encrypt_aes_ecb_sycl(benchmark::State& state) {
    Aes<Aes_var> aes(aes_key<Aes_var>); 
    AesTextEncrypt<Aes_var>::encrypt_ecb(q, aes, msg4k); // The first time takes an inordinate amount of time
    for (auto _ : state){
        AesTextEncrypt<Aes_var>::encrypt_ecb(q, aes, msg4k);
    }
}
BENCHMARK(msg4k_encrypt_aes_ecb_sycl<AesVariant::Aes128>);
BENCHMARK(msg4k_encrypt_aes_ecb_sycl<AesVariant::Aes192>);
BENCHMARK(msg4k_encrypt_aes_ecb_sycl<AesVariant::Aes256>);


template<AesVariant Aes_var>
static void msg4k_decrypt_aes_ecb_sycl(benchmark::State& state) {
    Aes<Aes_var> aes(aes_key<Aes_var>);
    auto decrypted_msg = AesTextEncrypt<Aes_var>::encrypt_ecb(aes, msg4k);
    for (auto _ : state){
        AesTextEncrypt<Aes_var>::decrypt_ecb(q, aes, decrypted_msg);
    }
}
BENCHMARK(msg4k_decrypt_aes_ecb_sycl<AesVariant::Aes128>);
BENCHMARK(msg4k_decrypt_aes_ecb_sycl<AesVariant::Aes192>);
BENCHMARK(msg4k_decrypt_aes_ecb_sycl<AesVariant::Aes256>);

#endif

template<AesVariant Aes_var>
static void msg4k_encrypt_aes_cbc(benchmark::State& state) {
    Aes<Aes_var> aes(aes_key<Aes_var>);
    for(auto _ : state){
        AesTextEncrypt<Aes_var>::encrypt_cbc(aes, msg4k, iv);
    }
}
BENCHMARK(msg4k_encrypt_aes_cbc<AesVariant::Aes128>);
BENCHMARK(msg4k_encrypt_aes_cbc<AesVariant::Aes192>);
BENCHMARK(msg4k_encrypt_aes_cbc<AesVariant::Aes256>);


template<AesVariant Aes_var>
static void msg4k_decrypt_aes_cbc(benchmark::State& state) {
    Aes<Aes_var> aes(aes_key<Aes_var>);
    auto decrypted_msg = AesTextEncrypt<Aes_var>::encrypt_cbc(aes, msg4k, iv);
    for(auto _ : state){
        AesTextEncrypt<Aes_var>::decrypt_cbc(aes, decrypted_msg, iv);
    }
}
BENCHMARK(msg4k_decrypt_aes_cbc<AesVariant::Aes128>);
BENCHMARK(msg4k_decrypt_aes_cbc<AesVariant::Aes192>);
BENCHMARK(msg4k_decrypt_aes_cbc<AesVariant::Aes256>);

BENCHMARK_MAIN();
