#ifndef OR_CONDITION_H
#define OR_CONDITION_H

#include "CompositeCondition.h"
#include <../resources/json.hpp>
#include <memory>


class OrCondition : public CompositeCondition {
public:
    OrCondition(std::unique_ptr<ConditionBase> lhs, std::unique_ptr<ConditionBase> rhs)
        : CompositeCondition(std::move(lhs), std::move(rhs)) {}

    bool validate() override;
    nlohmann::json toJson() const override;
};

#endif // OR_CONDITION_H
