#ifndef AND_CONDITION_H
#define AND_CONDITION_H

#include "CompositeCondition.h"
#include "../resources/json.hpp"
#include "ConditionBase.h"
#include <memory>

class AndCondition : public CompositeCondition {
public:
    AndCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs, std::chrono::milliseconds elapsedTime = std::chrono::milliseconds(MAX_ELAPSED_TIME));
    virtual ~AndCondition() = default;

    bool validate() override;
    nlohmann::json toJson() const override;
};

#endif 