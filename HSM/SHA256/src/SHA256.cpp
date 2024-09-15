// SHA-256 implementation
#include "SHA256.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

// SHA-256 constants (first 32 bits of the fractional parts of the cube roots of the first 64 primes)
const uint32_t K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Right rotation function
uint32_t rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

// SHA-256 functions
uint32_t choose(uint32_t e, uint32_t f, uint32_t g) {
    return (e & f) ^ (~e & g);
}

uint32_t majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a & b) ^ (a & c) ^ (b & c);
}

uint32_t sig0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32_t sig1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

uint32_t ep0(uint32_t x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

uint32_t ep1(uint32_t x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

// Padding the message according to SHA-256 standards
std::vector<uint8_t> padMessage(const std::string& message) {
    uint64_t bit_len = message.size() * 8;
    uint64_t padded_len = ((bit_len + 64) / 512 + 1) * 512;
    std::vector<uint8_t> padded_message(padded_len / 8, 0);
    std::copy(message.begin(), message.end(), padded_message.begin());
    padded_message[message.size()] = 0x80;

    for (int i = 0; i < 8; i++) {
        padded_message[padded_message.size() - 1 - i] = (bit_len >> (8 * i)) & 0xff;
    }

    return padded_message;
}

// SHA-256 hashing function
std::vector<uint32_t> sha256(const std::string& message) {
    std::vector<uint8_t> padded_message = padMessage(message);

    // Initial hash values (first 32 bits of the fractional parts of the square roots of the first 8 primes)
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    for (size_t i = 0; i < padded_message.size(); i += 64) {
        uint32_t w[64] = { 0 };
        for (int j = 0; j < 16; j++) {
            w[j] = (padded_message[i + 4 * j] << 24) | (padded_message[i + 4 * j + 1] << 16) | (padded_message[i + 4 * j + 2] << 8) | (padded_message[i + 4 * j + 3]);
        }

        for (int j = 16; j < 64; j++) {
            w[j] = sig1(w[j - 2]) + w[j - 7] + sig0(w[j - 15]) + w[j - 16];
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        for (int j = 0; j < 64; j++) {
            uint32_t temp1 = h + ep1(e) + choose(e, f, g) + K[j] + w[j];
            uint32_t temp2 = ep0(a) + majority(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    std::vector<uint32_t> hash = { h0, h1, h2, h3, h4, h5, h6, h7 };
    return hash;
}

// Converts the hash values to a hexadecimal string
std::string toHexString(const std::vector<uint32_t>& hash) {
    std::stringstream ss;
    for (uint32_t val : hash) {
        ss << std::hex << std::setw(8) << std::setfill('0') << val;
    }
    return ss.str();
}

// Compares two hash values
bool compareHashes(const std::vector<uint32_t>& hash1, const std::vector<uint32_t>& hash2) {
    if (hash1.size() != hash2.size()) return false;
    for (size_t i = 0; i < hash1.size(); i++) {
        if (hash1[i] != hash2[i]) return false;
    }
    return true;
}

