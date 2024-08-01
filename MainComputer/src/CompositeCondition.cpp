#include "CompositeCondition.h"

CompositeCondition::CompositeCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs)
    : LHS(lhs), RHS(rhs) {}

const std::shared_ptr<ConditionBase>& CompositeCondition::getLHS() const {
    return LHS;
}

const std::shared_ptr<ConditionBase>& CompositeCondition::getRHS() const {
    return RHS;
}
