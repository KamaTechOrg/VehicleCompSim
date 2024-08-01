#ifndef AND_CONDITION_H
#define AND_CONDITION_H

#include "CompositeCondition.h"
#include "../resources/json.hpp"
#include "ConditionBase.h"
#include <memory>


class AndCondition : public ConditionBase {
public:
    AndCondition(std::unique_ptr<ConditionBase> lhs, std::unique_ptr<ConditionBase> rhs);
    virtual ~AndCondition() = default;

    bool validate() override;
    nlohmann::json toJson() const override;

private:
    std::unique_ptr<ConditionBase> LHS;
    std::unique_ptr<ConditionBase> RHS;
};

#endif // AND_CONDITION_H
