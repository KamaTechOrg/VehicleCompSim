#ifndef CONDITION_BASE_H
#define CONDITION_BASE_H

#include <string>
#include "json.hpp"

/// <summary>
/// An abstract class that represents a condition, single or complex
/// </summary>
class ConditionBase {
public:
    virtual bool validate(const std::string &senderId, const std::string &value) const = 0;
    virtual nlohmann::json toJson() const = 0;
    virtual ~ConditionBase() = default;
};

#endif 
