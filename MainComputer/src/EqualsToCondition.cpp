#include "EqualsToCondition.h"
#include "json.hpp"

EqualsToCondition::EqualsToCondition(const std::string &senderId, const std::string &validationValue)
    : SimpleCondition(senderId, validationValue) {}

bool EqualsToCondition::validate(const std::string &senderId, const std::string &value) const {
    if (senderId != this->senderId) return false;
    
    return value == validationValue;
}

nlohmann::json EqualsToCondition::toJson() const {
    return {
        {"type", "EqualsToCondition"},
        {"senderId", senderId},
        {"validationValue", validationValue}
    };
}
