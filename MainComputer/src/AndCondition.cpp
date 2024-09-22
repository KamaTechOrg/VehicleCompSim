#include "AndCondition.h"


AndCondition::AndCondition(std::shared_ptr<ConditionBase> lhs,
    std::shared_ptr<ConditionBase> rhs,
    std::chrono::milliseconds elapsedTime)
    : CompositeCondition(lhs, rhs, elapsedTime),
    _startTime(std::chrono::steady_clock::now())
{
  
    assert((lhs && rhs) && "One or more arguments were nullptr");

}

bool AndCondition::validate(const std::string& senderId,
    const std::string& value) const
{
    auto currentTime = std::chrono::steady_clock::now();

    auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _startTime);

    if (timeDiff > _elapsedTime) {
        return false;
    }
    bool lhsValid = LHS ? LHS->validate(senderId, value) : false;
    bool rhsValid = RHS ? RHS->validate(senderId, value) : false;

    return lhsValid && rhsValid;
}

nlohmann::json AndCondition::toJson() const {
    return {
        {"type", "And"},
        {"lhs", LHS->toJson()},
        {"rhs", RHS->toJson()},
        {"elapsedTime", _elapsedTime.count()}
    };
}




