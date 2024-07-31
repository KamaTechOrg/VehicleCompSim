#include "CompositeCondition.h"

CompositeCondition::CompositeCondition(ConditionBase* lhs, ConditionBase* rhs)
    : LHS(lhs), RHS(rhs) {}

CompositeCondition::CompositeCondition(CompositeCondition* condition)
    : LHS(condition->LHS), RHS(condition->RHS) {}

CompositeCondition::~CompositeCondition() {
    delete LHS;
    delete RHS;
}