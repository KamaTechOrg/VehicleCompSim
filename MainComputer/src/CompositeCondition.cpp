#include "CompositeCondition.h"

CompositeCondition::CompositeCondition(std::shared_ptr<ConditionBase> lhs,
    std::shared_ptr<ConditionBase> rhs,
    std::chrono::milliseconds elapsedTime)
    : LHS(lhs), RHS(rhs), _elapsedTime(elapsedTime)
{
    if (!lhs || !rhs) {
        throw std::runtime_error("One or more arguments were nullptr");
    }
}

const std::shared_ptr<ConditionBase> &CompositeCondition::getLHS() const {
    return LHS;
}

const std::shared_ptr<ConditionBase> &CompositeCondition::getRHS() const {
    return RHS;
}