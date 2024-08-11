#include "GreaterThanCondition.h"
#include <stdexcept>
#include "json.hpp"

GreaterThanCondition::GreaterThanCondition(const std::string& senderId, const std::string& validationValue)
    : SimpleCondition(senderId, validationValue) {}

bool GreaterThanCondition::validate(const std::string& senderId, const std::string& value) const {
    if (senderId != this->senderId) return false;
    
    try {
        double numValue = std::stod(value);
        double numValidationValue = std::stod(validationValue);
        return numValue > numValidationValue;
    }
    catch (const std::invalid_argument&) {
        return false;
    }
}

nlohmann::json GreaterThanCondition::toJson() const {
    return {
        {"type", "GreaterThanCondition"},
        {"senderId", senderId},
        {"validationValue", validationValue}
    };
}
