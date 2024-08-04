#ifndef COMPOSITE_CONDITION_H
#define COMPOSITE_CONDITION_H

#include <memory>
#include <chrono>

#include "ConditionBase.h"
#include "constants.h"

class CompositeCondition : public ConditionBase {
public:
    CompositeCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs, std::chrono::milliseconds elapsedTime = std::chrono::milliseconds(MAX_ELAPSED_TIME));
    virtual ~CompositeCondition() = default;

    const std::shared_ptr<ConditionBase>& getLHS() const;
    const std::shared_ptr<ConditionBase>& getRHS() const;

protected:
    std::shared_ptr<ConditionBase> LHS;
    std::shared_ptr<ConditionBase> RHS;
    std::chrono::milliseconds _elapsedTime;
};

#endif // COMPOSITE_CONDITION_H
