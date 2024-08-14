#include "ConditionsManager.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <thread>
#include "json.hpp"
#include "constants.h"
#include <QtDebug>

ConditionsManager::ConditionsManager()
    : _isRunning(false)
{
    loadFromJson(CONDITIONS_JSON_FILE_NAME);
}

void ConditionsManager::run()
{
    _isRunning = true;
    std::thread([this]() {
        int count = 1;
        while (_isRunning)
        {
            qInfo() << "running " << count << "\n----- ----- ----- ----- -----\n";
            count++;
            // Sleep for 1 second to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            /*
            * TODO: listen to camera/sensors and validateAll() for each message.
            */
        }
        }).detach(); // Detach the thread so it runs independently
}

void ConditionsManager::stop()
{
    _isRunning = false;
    qInfo() << "stopping" << "\n----- ----- ----- ----- -----\n";
}

bool ConditionsManager::isRunning()
{
    return _isRunning;
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

    qInfo() << "JSON file ----- ----- ----- ----- -----\n" << j.dump(4) << "\n----- ----- ----- ----- -----\n";

    /*
    * TODO: create compsite conditions and push them into "conditions" member vector
    */
}
