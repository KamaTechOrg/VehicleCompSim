#include "ConditionsManager.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "json.hpp"
#include "constants.h"

ConditionsManager::ConditionsManager()
    : _isRunning(true)
{
    loadFromJson(CONDITIONS_JSON_FILE_NAME);
}

void ConditionsManager::run()
{
    while (_isRunning)
    {
        Sleep(1000); // temp, just to not stall the all program.
        /*
        * TODO: listen to camera/sensors and validateAll() for each message.
        */
    }
}

void ConditionsManager::stop()
{
    _isRunning = false;
}

void ConditionsManager::addCondition(std::shared_ptr<ConditionBase> condition)
{
    conditions.push_back(condition);
}

bool ConditionsManager::validateAll(const std::string& senderId, const std::string& value) const
{
    for (const auto& condition : conditions) {
        if (!condition->validate(senderId, value))
            return false;
    }
    return true;
}

void ConditionsManager::loadFromJson(const std::string& filename) const
{
    std::ifstream file(filename);
    if (!file.is_open())
        return; // TODO: maybe throw some error, but for now it's now handled anyway

    nlohmann::json j;
    file >> j;
    file.close();

    std::cout << j.dump(4) << std::endl;

    /*
    * TODO: create compsite conditions and push them into "conditions" member vector
    */
}
