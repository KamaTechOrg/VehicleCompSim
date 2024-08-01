#include "AndCondition.h"

AndCondition::AndCondition(std::unique_ptr<ConditionBase> lhs, std::unique_ptr<ConditionBase> rhs)
    : LHS(std::move(lhs)), RHS(std::move(rhs)) {}

bool AndCondition::validate() {
    return LHS->validate() && RHS->validate();
}

nlohmann::json AndCondition::toJson() const {
    return {
        {"type", "and"},
        {"left", LHS->toJson()},
        {"right", RHS->toJson()}
    };
}
