#include <benchmark/benchmark.h>
#include <cstddef>
#include <numeric>
#include "aes.hpp"
#include <array>

template<AesVariant AesVar>
static const typename Aes<AesVar>::KeyType aes_key;

template<>
static const Aes<AesVariant::Aes128>::KeyType aes_key<AesVariant::Aes128> = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                 0x09, 0xcf, 0x4f, 0x3c};

template<>
static const Aes<AesVariant::Aes192>::KeyType aes_key<AesVariant::Aes192> = {
      0xbb, 0x85, 0x57, 0x2b, 0x2b, 0x82, 0xce, 0xc0, 0x93, 0x3b, 0xa9,
      0x38, 0x55, 0x77, 0x9b, 0xd3, 0xd4, 0xca, 0xd5, 0x93, 0x5f, 0xc0,
      0x84, 0x85};


template<>
static const Aes<AesVariant::Aes256>::KeyType aes_key<AesVariant::Aes256> = {
      0xbb, 0x85, 0x57, 0x2b, 0x2b, 0x82, 0xce, 0xc0, 0x93, 0x3b, 0xa9,
      0x38, 0x55, 0x77, 0x9b, 0xd3, 0xd4, 0xca, 0xd5, 0x93, 0x5f, 0xc0,
      0x84, 0x85, 0x8d, 0xc3, 0x71, 0xb0, 0x65, 0x4a, 0x60, 0x31};


std::array<uint8_t, 16> iv = {0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30, 0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41};


static const std::string msg4k = []() -> std::string {
    std::string message(1<<12, '\0');
    std::iota(message.begin(), message.end(), 1);
    return message;
}();

template<AesVariant AesVar>
static void msg4k_encrypt_aes_ecb(benchmark::State& state) {
    Aes<AesVar> aes(aes_key<AesVar>);
    for (auto _ : state){
        aes.encrypt_ecb(msg4k);
    }
}
BENCHMARK(msg4k_encrypt_aes_ecb<AesVariant::Aes128>);
BENCHMARK(msg4k_encrypt_aes_ecb<AesVariant::Aes192>);
BENCHMARK(msg4k_encrypt_aes_ecb<AesVariant::Aes256>);


template<AesVariant AesVar>
static void msg4k_decrypt_aes_ecb(benchmark::State& state) {
    Aes<AesVar> aes(aes_key<AesVar>);
    auto decrypted_msg = aes.encrypt_ecb(msg4k);
    for (auto _ : state){
        aes.decrypt_ecb(decrypted_msg);
    }
}
BENCHMARK(msg4k_decrypt_aes_ecb<AesVariant::Aes128>);
BENCHMARK(msg4k_decrypt_aes_ecb<AesVariant::Aes192>);
BENCHMARK(msg4k_decrypt_aes_ecb<AesVariant::Aes256>);


template<AesVariant AesVar>
static void msg4k_decrypt_aes_cbc(benchmark::State& state) {
    Aes<AesVar> aes(aes_key<AesVar>);
    auto decrypted_msg = aes.encrypt_cbc(msg4k, iv);
    for (auto _ : state){
        aes.decrypt_cbc(decrypted_msg, iv);
    }
}
BENCHMARK(msg4k_decrypt_aes_cbc<AesVariant::Aes128>);
BENCHMARK(msg4k_decrypt_aes_cbc<AesVariant::Aes192>);
BENCHMARK(msg4k_decrypt_aes_cbc<AesVariant::Aes256>);

BENCHMARK_MAIN();
