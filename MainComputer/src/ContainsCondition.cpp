#include "ContainsCondition.h"
#include "json.hpp"

ContainsCondition::ContainsCondition(const std::string& senderId, const std::string& validationValue)
    : SimpleCondition(senderId, validationValue) {}

bool ContainsCondition::validate(const std::string& senderId, const std::string& value) const {
    if (senderId != this->senderId) return false;
    
    return value.find(validationValue) != std::string::npos;
}

nlohmann::json ContainsCondition::toJson() const {
    return {
        {"type", "ContainsCondition"},
        {"senderId", senderId},
        {"validationValue", validationValue}
    };
}
