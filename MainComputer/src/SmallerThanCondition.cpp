#include "SmallerThanCondition.h"
#include <stdexcept>
#include <../resources/json.hpp>

SmallerThanCondition::SmallerThanCondition(const std::string& senderId, const std::string& validationValue)
    : SimpleCondition(senderId, validationValue) {}

bool SmallerThanCondition::validate(const std::string& senderId, const std::string& value) const {
    if (senderId != this->senderId) {
        return false;
    }
    try {
        double numValue = std::stod(value);
        double numValidationValue = std::stod(validationValue);
        return numValue < numValidationValue;
    }
    catch (const std::invalid_argument&) {
        return false;
    }
}

nlohmann::json SmallerThanCondition::toJson() const {
    return {
        {"type", "SmallerThanCondition"},
        {"senderId", senderId},
        {"validationValue", validationValue}
    };
}
