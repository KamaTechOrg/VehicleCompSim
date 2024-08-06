#include "SimpleCondition.h"

SimpleCondition::SimpleCondition(const std::string& senderId, const std::string& validationValue)
    : senderId(senderId), validationValue(validationValue) {}

nlohmann::json SimpleCondition::toJson() const {
    return {
        {"type", "SimpleCondition"},
        {"senderId", senderId},
        {"validationValue", validationValue}
    };
}
