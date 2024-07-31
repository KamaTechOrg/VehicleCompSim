#ifndef COMPOSITE_CONDITION_H
#define COMPOSITE_CONDITION_H

#include "ConditionBase.h"

class CompositeCondition : public ConditionBase {
protected:
    ConditionBase* LHS;
    ConditionBase* RHS;

public:
    CompositeCondition(ConditionBase* lhs, ConditionBase* rhs);
    CompositeCondition(CompositeCondition* condition);
    virtual ~CompositeCondition();
};

#endif 