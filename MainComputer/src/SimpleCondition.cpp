#include "SimpleCondition.h"
#include <stdexcept>

SimpleCondition::SimpleCondition(const std::string& input, const std::string& validationType, const std::string& validationValue)
    : input(input), validationType(validationType), validationValue(validationValue)
{}

SimpleCondition::SimpleCondition(SimpleCondition* condition)
    : input(condition->input), validationType(condition->validationType), validationValue(condition->validationValue)
{}

bool SimpleCondition::validate() {
    if (validationType == "equals") {
        return input == validationValue;
    }
    else if (validationType == "not_equals") {
        return input != validationValue;
    }
    else if (validationType == "contains") {
        return input.find(validationValue) != std::string::npos;
    }
    else if (validationType == "greater_than") {
        return std::stoi(input) > std::stoi(validationValue);
    }
    else if (validationType == "less_than") {
        return std::stoi(input) < std::stoi(validationValue);
    }
    else if (validationType == "greater_than_or_equal") {
        return std::stoi(input) >= std::stoi(validationValue);
    }
    else if (validationType == "less_than_or_equal") {
        return std::stoi(input) <= std::stoi(validationValue);
    }
    throw std::runtime_error("Unknown validation type: " + validationType);
}

nlohmann::json SimpleCondition::toJson() const {
    return {
        {"input", input},
        {"validationType", validationType},
        {"validationValue", validationValue}
    };
}