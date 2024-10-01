#include "OrCondition.h"

OrCondition::OrCondition(std::shared_ptr<ConditionBase> lhs,
                         std::shared_ptr<ConditionBase> rhs,
                         std::chrono::milliseconds elapsedTime)
    : CompositeCondition(lhs, rhs, elapsedTime) {}

bool OrCondition::validate(const std::string &senderId, const std::string &value) const {
    bool lhsValid = LHS ? LHS->validate(senderId, value) : false;
    bool rhsValid = RHS ? RHS->validate(senderId, value) : false;
    return lhsValid || rhsValid;
}

nlohmann::json OrCondition::toJson() const {
    return {
        {"type", "Or"},
        {"lhs", LHS->toJson()},
        {"rhs", RHS->toJson()},
        {"elapsedTime", _elapsedTime.count()}
    };
}