#pragma once

#ifndef SHA256_H
#define SHA256_H

#include <vector>
#include <string>
#include <cstdint>
namespace SHA256 {

std::vector<uint32_t> sha256(const std::string& message);
bool compareHashes(const std::vector<uint32_t>& hash1, const std::vector<uint32_t>& hash2);
std::string toHexString(const std::vector<uint32_t>& hash);

}

using namespace SHA256;

#endif // SHA256_H

