#ifndef SMALLER_THAN_CONDITION_H
#define SMALLER_THAN_CONDITION_H

#include "SimpleCondition.h"

class SmallerThanCondition : public SimpleCondition {
public:
    SmallerThanCondition(const std::string &senderId, const std::string &validationValue);
    bool validate(const std::string &senderId, const std::string &value) const override;
    nlohmann::json toJson() const override;
};

#endif 
