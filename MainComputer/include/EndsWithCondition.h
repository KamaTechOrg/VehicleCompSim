#ifndef ENDS_WITH_CONDITION_H
#define ENDS_WITH_CONDITION_H

#include "SimpleCondition.h"

class EndsWithCondition : public SimpleCondition {
public:
    EndsWithCondition(const std::string &senderId, const std::string &validationValue);
    bool validate(const std::string &senderId, const std::string &value) const override;
    nlohmann::json toJson() const override;
};

#endif