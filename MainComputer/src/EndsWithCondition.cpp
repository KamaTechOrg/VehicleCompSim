#include "EndsWithCondition.h"
#include "json.hpp"

EndsWithCondition::EndsWithCondition(const std::string &senderId, const std::string &validationValue)
    : SimpleCondition(senderId, validationValue) {}

bool EndsWithCondition::validate(const std::string &senderId, const std::string &value) const {
    if (senderId != this->senderId) return false;
    
    if (validationValue.length() > value.length()) return false;
    
    return std::equal(validationValue.rbegin(), validationValue.rend(), value.rbegin());
}

nlohmann::json EndsWithCondition::toJson() const {
    return {
        {"type", "EndsWithCondition"},
        {"senderId", senderId},
        {"validationValue", validationValue}
    };
}
