#ifndef CONDITIONS_MANAGER_H
#define CONDITIONS_MANAGER_H

#include "ConditionBase.h"
#include <vector>
#include <string>
#include <memory>
#include "json.hpp"

class ConditionsManager {
private:
    static std::vector<std::shared_ptr<ConditionBase>> conditions;
    // static becuase we want the save button from the UI to be able to reload the conditions from the saved JSON file

    void addCondition(std::shared_ptr<ConditionBase> condition);
    bool validateAll(const std::string& senderId, const std::string& value) const;
    bool _isRunning;

public:
    ConditionsManager();
    void run();
    void stop();
    bool isRunning();
    void loadFromJson(const std::string& filename);
};

#endif