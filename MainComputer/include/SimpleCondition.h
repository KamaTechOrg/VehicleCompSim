#ifndef SIMPLE_CONDITION_H
#define SIMPLE_CONDITION_H

#include "ConditionBase.h"
#include <string>
#include <memory>

class SimpleCondition : public ConditionBase {
private:
    std::string input;
    std::string validationType;
    std::string validationValue;

public:
    SimpleCondition() = delete;
    SimpleCondition(const std::string& input, const std::string& validationType, const std::string& validationValue);
    SimpleCondition(std::shared_ptr<SimpleCondition> condition);
    bool validate() override;
    nlohmann::json toJson() const override;
};

#endif
