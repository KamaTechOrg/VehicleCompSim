#include "SimpleCondition.h"
#include <stdexcept>
#include <memory>

enum class ValidationType {
    EqualsTo,
    GreaterThan,
    SmallerThan,
    StartsWith,
    EndsWith,
    Unknown
};

ValidationType stringToValidationType(const std::string& str) {
    if (str == "equals to") return ValidationType::EqualsTo;
    if (str == "greater then") return ValidationType::GreaterThan;
    if (str == "smaller then") return ValidationType::SmallerThan;
    if (str == "starts with") return ValidationType::StartsWith;
    if (str == "ends with") return ValidationType::EndsWith;
    return ValidationType::Unknown;
}

SimpleCondition::SimpleCondition(const std::string& input, const std::string& validationType, const std::string& validationValue)
    : input(input), validationType(validationType), validationValue(validationValue)
{}

SimpleCondition::SimpleCondition(std::shared_ptr<SimpleCondition> condition)
    : input(condition->input), validationType(condition->validationType), validationValue(condition->validationValue)
{}

bool SimpleCondition::validate() {
    switch (stringToValidationType(validationType)) {
    case ValidationType::EqualsTo:
        return input == validationValue;
    case ValidationType::GreaterThan:
        return std::stoi(input) > std::stoi(validationValue);
    case ValidationType::SmallerThan:
        return std::stoi(input) < std::stoi(validationValue);
    case ValidationType::StartsWith:
        return input.find(validationValue) == 0;
    case ValidationType::EndsWith:
        return input.rfind(validationValue) == (input.size() - validationValue.size());
    default:
        throw std::runtime_error("Unknown validation type: " + validationType);
    }
}

nlohmann::json SimpleCondition::toJson() const {
    return {
        {"input", input},
        {"validationType", validationType},
        {"validationValue", validationValue}
    };
}
