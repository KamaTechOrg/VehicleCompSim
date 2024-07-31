#pragma once

#ifndef SHA256_H
#define SHA256_H

#include <vector>
#include <string>
#include <cstdint>

std::vector<uint32_t> sha256(const std::string& message);
bool compareHashes(const std::vector<uint32_t>& hash1, const std::vector<uint32_t>& hash2);
std::string toHexString(const std::vector<uint32_t>& hash);
void runTests();


#endif // SHA256_H

