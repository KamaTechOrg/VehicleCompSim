#ifndef CONDITION_BASE_H
#define CONDITION_BASE_H

#include <../resources/json.hpp>

class ConditionBase {
public:
    ConditionBase() = default;
    virtual bool validate() = 0;
    virtual nlohmann::json toJson() const = 0;
    virtual ~ConditionBase() = default;
};

#endif 