#ifndef COMPOSITE_CONDITION_H
#define COMPOSITE_CONDITION_H
#include <memory>
#include <chrono>
#include "ConditionBase.h"
#include "constants.h"

/// <summary>
/// Represents a condition that contains two other conditions
/// It alloweds to validate() the conditions and evaluate them
/// this is a pure virtual class
/// The classes AndCondition and Orcondition inherit from it and implement it 
/// </summary>
class CompositeCondition : public ConditionBase {
public:
    CompositeCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs, std::chrono::milliseconds elapsedTime = std::chrono::milliseconds(constants::MAX_ELAPSED_TIME));
    virtual ~CompositeCondition() = default;
    bool validate(const std::string &senderId, const std::string &value) const override = 0;
    const std::shared_ptr<ConditionBase>& getLHS() const;
    const std::shared_ptr<ConditionBase>& getRHS() const;
    nlohmann::json toJson() const override = 0;

protected:
    std::shared_ptr<ConditionBase> LHS;
    std::shared_ptr<ConditionBase> RHS;
    std::chrono::milliseconds _elapsedTime;
};
#endif