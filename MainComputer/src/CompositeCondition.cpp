#include "CompositeCondition.h"

CompositeCondition::CompositeCondition(std::shared_ptr<ConditionBase> lhs,
                                       std::shared_ptr<ConditionBase> rhs,
                                       std::chrono::milliseconds elapsedTime)
    : LHS(lhs), RHS(rhs), _elapsedTime(elapsedTime) {}

const std::shared_ptr<ConditionBase>& CompositeCondition::getLHS() const {
    return LHS;
}

const std::shared_ptr<ConditionBase>& CompositeCondition::getRHS() const {
    return RHS;
}