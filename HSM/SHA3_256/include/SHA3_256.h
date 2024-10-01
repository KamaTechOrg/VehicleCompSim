#pragma once

#ifndef SHA3_256_H
#define SHA3_256_H

#include <vector>
#include <string>
#include <cstdint>

std::vector<uint32_t> sha3_256(const std::string& message);
bool compareHashes(const std::vector<uint32_t>& hash1, const std::vector<uint32_t>& hash2);
std::string toHexString(const std::vector<uint32_t>& hash);

#endif // SHA3_256_H

