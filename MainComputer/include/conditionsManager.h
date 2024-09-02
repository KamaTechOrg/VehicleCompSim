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

    void addCondition(std::shared_ptr<ConditionBase> condition);
    bool validateAll(const std::string& senderId, const std::string& value) const;
    void loadFromJson(const std::string& filename);
    bool _isRunning;

public:
    ConditionsManager();
    void run();
    void stop();
    bool isRunning();
};

#endif