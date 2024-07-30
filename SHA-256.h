#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstring>

std::vector<uint32_t> sha256(const std::string& message);

std::string toHexString(const std::vector<uint32_t>& hash);