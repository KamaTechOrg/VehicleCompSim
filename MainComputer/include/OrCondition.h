#ifndef OR_CONDITION_H
#define OR_CONDITION_H

#include "CompositeCondition.h"
#include "json.hpp"
#include <memory>
#include <chrono>

class OrCondition : public CompositeCondition {
public:
    OrCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs, std::chrono::milliseconds elapsedTime = std::chrono::milliseconds(MAX_ELAPSED_TIME));
    virtual ~OrCondition() = default;
    bool validate(const std::string& senderId, const std::string& value) const override;
    nlohmann::json toJson() const override;
};

#endif