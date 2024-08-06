#ifndef GREATER_THAN_CONDITION_H
#define GREATER_THAN_CONDITION_H

#include "SimpleCondition.h"

class GreaterThanCondition : public SimpleCondition {
public:
    GreaterThanCondition(const std::string& senderId, const std::string& validationValue);
    bool validate(const std::string& senderId, const std::string& value) const override;
    nlohmann::json toJson() const override;
};

#endif 