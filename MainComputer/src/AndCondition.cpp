#include "AndCondition.h"

AndCondition::AndCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs, std::chrono::milliseconds elapsedTime)
    : CompositeCondition(lhs, rhs, elapsedTime) {}

bool AndCondition::validate() {
    return getLHS()->validate() && getRHS()->validate();
}

nlohmann::json AndCondition::toJson() const {
    return {
        {"type", "AndCondition"},
        {"elapsedTime", std::to_string(_elapsedTime.count())},
        {"LHS", getLHS()->toJson()},
        {"RHS", getRHS()->toJson()}
    };
}
