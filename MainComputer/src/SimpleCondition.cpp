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

SimpleCondition::SimpleCondition(const std::string& input, const std::string& validationType, const std::string& validationValue)
    : input(input), validationType(validationType), validationValue(validationValue)
{}

SimpleCondition::SimpleCondition(std::shared_ptr<SimpleCondition> condition)
    : input(condition->input), validationType(condition->validationType), validationValue(condition->validationValue)
{}

bool SimpleCondition::validate() {
    ValidationType type;

    if (validationType == "equals to") type = ValidationType::EqualsTo;
    else if (validationType == "greater than") type = ValidationType::GreaterThan;
    else if (validationType == "smaller than") type = ValidationType::SmallerThan;
    else if (validationType == "starts with") type = ValidationType::StartsWith;
    else if (validationType == "ends with") type = ValidationType::EndsWith;
    else type = ValidationType::Unknown;

    switch (type) {
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
