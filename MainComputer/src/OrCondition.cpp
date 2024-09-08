#include "OrCondition.h"

OrCondition::OrCondition(std::shared_ptr<ConditionBase> lhs,
                         std::shared_ptr<ConditionBase> rhs,
                         std::chrono::milliseconds elapsedTime)
    : CompositeCondition(lhs, rhs, elapsedTime) {}

bool OrCondition::validate(const std::string &senderId, const std::string &value) const {
    return LHS->validate(senderId, value) || RHS->validate(senderId, value);
}

nlohmann::json OrCondition::toJson() const {
    return {
        {"type", "Or"},
        {"lhs", LHS->toJson()},
        {"rhs", RHS->toJson()},
        {"elapsedTime", _elapsedTime.count()}
    };
}