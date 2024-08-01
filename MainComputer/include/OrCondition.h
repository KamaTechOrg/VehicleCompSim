#ifndef OR_CONDITION_H
#define OR_CONDITION_H

#include "CompositeCondition.h"
#include <../resources/json.hpp>
#include <memory>


class OrCondition : public CompositeCondition {
public:
    OrCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs)
        : CompositeCondition(lhs,rhs) {}

    bool validate() override;
    nlohmann::json toJson() const override;
};

#endif
