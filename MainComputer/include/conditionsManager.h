#ifndef CONDITIONS_MANAGER_H
#define CONDITIONS_MANAGER_H

#include "ConditionBase.h"
#include <vector>
#include <string>
#include <memory>
#include "json.hpp"

class ConditionsManager {
private:
    std::vector<std::shared_ptr<ConditionBase>> conditions;

public:
    void addCondition(std::shared_ptr<ConditionBase> condition);
    bool validateAll() const;
    void exportToJson(const std::string& filename) const;
};

#endif