#ifndef COMPOSITE_CONDITION_H
#define COMPOSITE_CONDITION_H

#include <memory>
#include "ConditionBase.h"

class CompositeCondition : public ConditionBase {
public:
    CompositeCondition(std::unique_ptr<ConditionBase> lhs, std::unique_ptr<ConditionBase> rhs);
    virtual ~CompositeCondition() = default;

    // גישה לאובייקטים המוגדרים ב-CompositeCondition
    std::unique_ptr<ConditionBase>& getLHS();
    std::unique_ptr<ConditionBase>& getRHS();

protected:
    std::unique_ptr<ConditionBase> LHS;
    std::unique_ptr<ConditionBase> RHS;
};

#endif 
