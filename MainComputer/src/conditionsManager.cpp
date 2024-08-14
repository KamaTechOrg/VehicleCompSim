#include "ConditionsManager.h"
#include <stdexcept>
#include <fstream>

ConditionsManager::ConditionsManager()
{
    /*
    * TODO: loadFromJson();
    */
}

void ConditionsManager::run()
{
    /*
    * TODO: listen to camera/sensors and validateAll() for each message.
    */
}

void ConditionsManager::addCondition(std::shared_ptr<ConditionBase> condition) {
    assert(condition != nullptr);
    conditions.push_back(condition);
}

bool ConditionsManager::validateAll() const {
    for (const auto &condition : conditions) {
        if (!condition->validate()) return false;
    }
    return true;
}

void ConditionsManager::loadFromJson(const std::string& filename) const {
    /*
    * filename = "conditions.json"
    */
}
