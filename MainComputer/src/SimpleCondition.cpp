#include "SimpleCondition.h"
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <algorithm>

enum class ValidationType {
    EqualsTo,
    GreaterThan,
    SmallerThan,
    StartsWith,
    EndsWith,
    Contains,
    Unknown
};

static const std::unordered_map<std::string, ValidationType> validationTypeMap = {
    {"equals to", ValidationType::EqualsTo},
    {"greater than", ValidationType::GreaterThan},
    {"smaller than", ValidationType::SmallerThan},
    {"starts with", ValidationType::StartsWith},
    {"ends with", ValidationType::EndsWith},
    {"contains", ValidationType::Contains}
};

SimpleCondition::SimpleCondition(const std::string& input, const std::string& validationType, const std::string& validationValue)
    : input(input), validationType(validationType), validationValue(validationValue)
{}

SimpleCondition::SimpleCondition(std::shared_ptr<SimpleCondition> condition)
    : input(condition->input), validationType(condition->validationType), validationValue(condition->validationValue)
{}

bool SimpleCondition::validate() {
    ValidationType type = ValidationType::Unknown;
    auto it = validationTypeMap.find(validationType);
    if (it != validationTypeMap.end()) {
        type = it->second;
    }
    try {
        switch (type) {
        case ValidationType::EqualsTo:
            return input == validationValue;
        case ValidationType::GreaterThan:
        case ValidationType::SmallerThan: {
            int inputVal = std::stoi(input);
            int validationVal = std::stoi(validationValue);
            return (type == ValidationType::GreaterThan) ? inputVal > validationVal : inputVal < validationVal;
        }
        case ValidationType::StartsWith:
            return input.find(validationValue) == 0;
        case ValidationType::EndsWith:
            return (input.size() >= validationValue.size()) &&
                (input.rfind(validationValue) == (input.size() - validationValue.size()));
        case ValidationType::Contains:
            return input.find(validationValue) != std::string::npos;
        default:
            throw std::runtime_error("Unknown validation type: " + validationType);
        }
    }
    catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid number format for comparison: " + std::string(e.what()));
    }
    catch (const std::out_of_range& e) {
        throw std::runtime_error("Number out of range for comparison: " + std::string(e.what()));
    }
}

nlohmann::json SimpleCondition::toJson() const {
    return {
        {"input", input},
        {"validationType", validationType},
        {"validationValue", validationValue}
    };
}