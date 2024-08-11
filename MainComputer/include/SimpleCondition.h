#ifndef SIMPLE_CONDITION_H
#define SIMPLE_CONDITION_H

#include "ConditionBase.h"
#include "json.hpp"
#include <string>

class SimpleCondition : public ConditionBase {
public:
    SimpleCondition(const std::string& senderId, const std::string& validationValue);
    virtual ~SimpleCondition() = default;

    virtual bool validate(const std::string& senderId, const std::string& value) const override = 0;
    nlohmann::json toJson() const override;

protected:
    std::string senderId;
    std::string validationValue;
};

#endif 
