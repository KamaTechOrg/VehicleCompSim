#ifndef COMPOSITE_CONDITION_H
#define COMPOSITE_CONDITION_H

#include <memory>
#include "ConditionBase.h"

class CompositeCondition : public ConditionBase {
public:
    CompositeCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs);
    virtual ~CompositeCondition() = default;

    const std::shared_ptr<ConditionBase>& getLHS() const;
    const std::shared_ptr<ConditionBase>& getRHS() const;

protected:
    std::shared_ptr<ConditionBase> LHS;
    std::shared_ptr<ConditionBase> RHS;
};

#endif // COMPOSITE_CONDITION_H
