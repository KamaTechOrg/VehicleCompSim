#include "OrCondition.h"

OrCondition::OrCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs, std::chrono::milliseconds elapsedTime)
    : CompositeCondition(lhs, rhs, elapsedTime) {}

bool OrCondition::validate() {
    return getLHS()->validate() || getRHS()->validate();
}

nlohmann::json OrCondition::toJson() const {
    return {
        {"type", "OrCondition"},
        {"elapsedTime", std::to_string(_elapsedTime.count())},
        {"LHS", getLHS()->toJson()},
        {"RHS", getRHS()->toJson()}
    };
}
