#ifndef CONDITIONS_MANAGER_H
#define CONDITIONS_MANAGER_H

#include "ConditionBase.h"
#include "Communication.h"
#include "Action.h" 

#include <vector>
#include <string>
#include <memory>
#include <unordered_map> 
#include "json.hpp"


class ConditionsManager {
private:
    static std::vector<std::shared_ptr<ConditionBase>> conditions;
    static std::vector<std::vector<Action>> actions;

    void addCondition(std::shared_ptr<ConditionBase> condition);
    void addAction(const int index, const Action& action);
    bool validateAll(const std::string &senderId, const std::string &value) const;
    void executeActions(const int index);

    bool _isRunning;

public:
    ConditionsManager();
    void run();
    void stop();
    bool isRunning();
    void loadFromJson();
    std::pair<std::string, std::string> parseMessage(const std::string &message);  // Parse the message to extract ID and value
};

#endif