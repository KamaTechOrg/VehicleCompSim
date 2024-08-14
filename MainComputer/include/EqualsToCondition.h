#ifndef EQUALS_TO_CONDITION_H
#define EQUALS_TO_CONDITION_H

#include "SimpleCondition.h"

class EqualsToCondition : public SimpleCondition {
public:
    EqualsToCondition(const std::string &senderId, const std::string &validationValue);
    bool validate(const std::string &senderId, const std::string &value) const override;
    nlohmann::json toJson() const override;
};

#endif
