#include "AndCondition.h"


AndCondition::AndCondition(std::shared_ptr<ConditionBase> lhs,
                           std::shared_ptr<ConditionBase> rhs,
                           std::chrono::milliseconds elapsedTime)
    : CompositeCondition(lhs, rhs, elapsedTime)
{
}

bool AndCondition::validate(const std::string& senderId,
                            const std::string& value) const
{
    return LHS->validate(senderId, value) && RHS->validate(senderId, value);
}

nlohmann::json AndCondition::toJson() const {
    return {
        {"type", "AndCondition"},
        {"lhs", LHS->toJson()},
        {"rhs", RHS->toJson()},
        {"elapsedTime", _elapsedTime.count()}
    };
}