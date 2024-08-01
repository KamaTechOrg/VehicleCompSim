#include "CompositeCondition.h"

CompositeCondition::CompositeCondition(std::unique_ptr<ConditionBase> lhs, std::unique_ptr<ConditionBase> rhs)
    : LHS(std::move(lhs)), RHS(std::move(rhs)) {}

std::unique_ptr<ConditionBase>& CompositeCondition::getLHS() {
    return LHS;
}

std::unique_ptr<ConditionBase>& CompositeCondition::getRHS() {
    return RHS;
}
