#ifndef AND_CONDITION_H
#define AND_CONDITION_H

#include "CompositeCondition.h"
#include "json.hpp"
#include <memory>
#include <chrono>

// This class represents an And-Condition,
// it contains two other coditions, and on
// validate() it checks if both of the are met.

// It also features an elipsed time that can
// between the two conditions validation.
class AndCondition : public CompositeCondition {
public:
    AndCondition(std::shared_ptr<ConditionBase> lhs, std::shared_ptr<ConditionBase> rhs, std::chrono::milliseconds elapsedTime = std::chrono::milliseconds(constants::MAX_ELAPSED_TIME));
    virtual ~AndCondition() = default;
    bool validate(const std::string& senderId, const std::string& value) const override;
    nlohmann::json toJson() const override;

private:
    mutable std::chrono::steady_clock::time_point _startTime;
};

#endif 