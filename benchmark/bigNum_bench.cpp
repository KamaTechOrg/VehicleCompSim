#include <benchmark/benchmark.h>
#include "BigNum.h"

// Define another benchmark
static void BN_128_bit_modulo(benchmark::State& state) {
    std::string expected = "1"; // ffffffffffffffffffffffffffffffff % 2 = 1 in hex
    BigNum f2("ffffffffffffffffffffffffffffffff");
    BigNum g2("2");
    for (auto _ : state){
        BigNum h2 = f2 % g2;
    }
}
BENCHMARK(BN_128_bit_modulo);


static void BN_128_bit_division(benchmark::State& state) {
    std::string expected = "1"; // ffffffffffffffffffffffffffffffff % 2 = 1 in hex
    BigNum f2("ffffffffffffffffffffffffffffffff");
    BigNum g2("2");
    for (auto _ : state){
        BigNum h2 = f2 / g2;
    }
}
BENCHMARK(BN_128_bit_division);

static void BN_128_bit_addition(benchmark::State& state) {

    std::string expected = "100000000000000000000000000000000"; // ffffffffffffffffffffffffffffffff + 1 = 100000000000000000000000000000000 in hex
    BigNum t1("ffffffffffffffffffffffffffffffff");
    BigNum u1("1");
    for (auto _ : state){
        BigNum v1 = t1 + u1;
    }
}
BENCHMARK(BN_128_bit_addition);


static void BN_128_bit_subtraction(benchmark::State& state) {

    BigNum w1("ffffffffffffffffffffffffffffffff");
    BigNum x1("1");
    for (auto _ : state){
        BigNum v1 = w1 - x1;
    }
}
BENCHMARK(BN_128_bit_subtraction);


static void BN_multiplication_128_bit(benchmark::State& state) {
    BigNum w1("ffffffffffffffffffffffffffffffff");
    BigNum x1("2");
    for (auto _ : state){
        BigNum v1 = w1 * x1;
    }
}
BENCHMARK(BN_multiplication_128_bit);

static void BN_left_shift_128_bit(benchmark::State& state) {
    BigNum w1("ffffffffffffffffffffffffffffffff");
    for (auto _ : state){
        BigNum v1 = w1 << 3;
    }
}
BENCHMARK(BN_left_shift_128_bit);

static void BN_right_shift_128_bit(benchmark::State& state) {
    BigNum w1("ffffffffffffffffffffffffffffffff");
    for (auto _ : state){
        BigNum v1 = w1 >> 3;
    }
}
BENCHMARK(BN_right_shift_128_bit);


BENCHMARK_MAIN();
