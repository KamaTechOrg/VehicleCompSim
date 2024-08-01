#include "AndCondition.h"

AndCondition::AndCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs)
    : CompositeCondition(lhs, rhs) {}

bool AndCondition::validate() {
    return getLHS()->validate() && getRHS()->validate();
}

nlohmann::json AndCondition::toJson() const {
    return {
        {"type", "AndCondition"},
        {"LHS", getLHS()->toJson()},
        {"RHS", getRHS()->toJson()}
    };
}
