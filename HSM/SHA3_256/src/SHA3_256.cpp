#include "SHA3_256.h"
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstring>

constexpr int BITRATE = 1088;
constexpr int CAPACITY = 512;
constexpr int STATE_SIZE = 1600 / 64;
constexpr int SHA3_256_DIGEST_SIZE = 32;
constexpr int RATE_IN_BYTES = BITRATE / 8;

typedef uint64_t UINT64;

static const UINT64 KeccakRoundConstants[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808AULL, 0x8000000080008000ULL,
    0x000000000000808BULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008AULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000AULL,
    0x000000008000808BULL, 0x800000000000008BULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800AULL, 0x800000008000000AULL,
    0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

// פונקציית סיבוב שמאלית (left rotate)
inline UINT64 ROTL64(UINT64 x, int n) {
    return (x << n) | (x >> (64 - n));
}

// פונקציית Keccak-f
void keccak_f(UINT64 state[25]) {
    for (int round = 0; round < 24; ++round) {
        UINT64 C[5], D[5];

        // θ
        for (int x = 0; x < 5; ++x) {
            C[x] = state[x] ^ state[x + 5] ^ state[x + 10] ^ state[x + 15] ^ state[x + 20];
        }
        for (int x = 0; x < 5; ++x) {
            D[x] = C[(x + 4) % 5] ^ ROTL64(C[(x + 1) % 5], 1);
            for (int y = 0; y < 5; ++y) {
                state[x + 5 * y] ^= D[x];
            }
        }

        // ρ ו-π
        UINT64 B[25];
        for (int x = 0; x < 5; ++x) {
            for (int y = 0; y < 5; ++y) {
                B[y + 5 * ((2 * x + 3 * y) % 5)] = ROTL64(state[x + 5 * y], ((x == 0 && y == 0) ? 0 : (x + y + 1) % 64));
            }
        }

        // χ
        for (int x = 0; x < 5; ++x) {
            for (int y = 0; y < 5; ++y) {
                state[x + 5 * y] = B[x + 5 * y] ^ (~B[(x + 1) % 5 + 5 * y] & B[(x + 2) % 5 + 5 * y]);
            }
        }

        // ι
        state[0] ^= KeccakRoundConstants[round];
    }
}

// Padding לפי תקן SHA3
void sha3_pad(std::vector<uint8_t>& input) {
    input.push_back(0x06);                    // ניהול padding בתקן Keccak
    input.resize(((input.size() + RATE_IN_BYTES - 1) / RATE_IN_BYTES) * RATE_IN_BYTES, 0);
    input.back() |= 0x80;                     // הגדרת ה-padding הסופי
}

// מימוש SHA3-256
std::vector<uint8_t> sha3_256(const std::vector<uint8_t>& input) {
    UINT64 state[STATE_SIZE] = {0};           // מצב התחלתי
    size_t rate_bytes = RATE_IN_BYTES;
    size_t block_size = 0;

    // ספיגת מידע
    std::vector<uint8_t> padded_input = input;
    sha3_pad(padded_input);

    for (size_t i = 0; i < padded_input.size(); ++i) {
        reinterpret_cast<uint8_t*>(state)[i] ^= padded_input[i];

        if (++block_size == rate_bytes) {
            keccak_f(state);
            block_size = 0;
        }
    }

    // חילוץ תמצית הגיבוב
    std::vector<uint8_t> digest(SHA3_256_DIGEST_SIZE);
    std::memcpy(digest.data(), state, SHA3_256_DIGEST_SIZE);

    return digest;
}

// פונקציית בדיקה - השוואת תוצאה ידועה
bool verify_sha3_256(const std::string& input_str, const std::string& expected_hex) {
    std::vector<uint8_t> input(input_str.begin(), input_str.end());
    std::vector<uint8_t> digest = sha3_256(input);

    std::ostringstream result;
    for (auto byte : digest) {
        result << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }

    return result.str() == expected_hex;
}


