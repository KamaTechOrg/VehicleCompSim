#include "StartsWithCondition.h"
#include <../resources/json.hpp>

StartsWithCondition::StartsWithCondition(const std::string& senderId, const std::string& validationValue)
    : SimpleCondition(senderId, validationValue) {}

bool StartsWithCondition::validate(const std::string& senderId, const std::string& value) const {
    if (senderId != this->senderId) {
        return false;
    }
    return value.compare(0, validationValue.length(), validationValue) == 0;
}

nlohmann::json StartsWithCondition::toJson() const {
    return {
        {"type", "StartsWithCondition"},
        {"senderId", senderId},
        {"validationValue", validationValue}
    };
}
