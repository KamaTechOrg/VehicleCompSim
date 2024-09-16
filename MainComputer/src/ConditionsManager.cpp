#include "ConditionsManager.h"
#include "constants.h"
#include "ConditionsFactory.h"
#include "Action.h" 
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <thread>
#include <QtDebug>
#include <regex> 
#include "json.hpp"
#include <algorithm> 


std::vector<std::shared_ptr<ConditionBase>> ConditionsManager::conditions;
std::unordered_map<std::string, Action> ConditionsManager::actions;

ConditionsManager::ConditionsManager()
    : _isRunning(false)
{
    loadFromJson(constants::CONDITIONS_JSON_FILE_NAME);
}

void ConditionsManager::addAction(const std::string &id, const Action &action)
{
    actions[id] = action;
}


void ConditionsManager::executeAction(const std::string &id)
{
    auto it = actions.find(id);  
    if (it != actions.end()) {
        const Action& action = it->second;  
        Communication communication;
        int targetUnitAsInt = std::stoi(action.getTargetUnit());

        communication.sendTo(targetUnitAsInt, action.getMessageToSend());
        qInfo() << "Action executed: Sent message to ID:" << id.c_str();
    }
    else {
        qWarning() << "No action found for ID:" << id.c_str();
    }
}

// Helper function to parse messages with spaces in ID and VALUE
std::pair<std::string, std::string> ConditionsManager::parseMessage(const std::string &message)
{
    // message format: "ID:some id, VALUE:some value"
    std::regex messageRegex(R"(ID:([^,]+),VALUE:(.+))");
    std::smatch matches;

    if (std::regex_search(message, matches, messageRegex) && matches.size() == 3) {
        return { matches[1].str(), matches[2].str() };
    }
    else {
        throw std::runtime_error("Invalid message format");
    }
}


void ConditionsManager::run()
{
    _isRunning = true;
    std::thread([this]() {
        Communication communication;
        communication.connectToSensors();

        std::string count;
        while (_isRunning)
        {
            qInfo() << "running";
            std::string message = communication.getMessageFromQueue();
            try {
                auto [id, value] = parseMessage(message);

                if (validateAll(id, value)) {
                    qInfo() << "Validation succeeded for ID:" << id.c_str() << " with value:" << value.c_str();
                    executeAction(id);  
                }
                else {
                    qInfo() << "Validation failed for ID:" << id.c_str() << " with value:" << value.c_str();
                }
            }
            catch (const std::exception& e) {
                qWarning() << "Failed to parse message:" << e.what();
            }
        }
        qInfo() << "Conditions Manager thread stopping";
        }).detach(); // Detach the thread so it runs independently*/
}

void ConditionsManager::stop()
{
    _isRunning = false;
    qInfo() << "stopping" << "\n----- ----- ----- ----- -----\n";
}

bool ConditionsManager::isRunning()
{
    qInfo() << "Running" << "\n----- ----- ----- ----- -----\n";
    return _isRunning;
}

void ConditionsManager::addCondition(std::shared_ptr<ConditionBase> condition)
{
    conditions.push_back(condition);
}

bool ConditionsManager::validateAll(const std::string& senderId, const std::string& value) const
{
    for (const auto& condition : conditions) {
        if (condition == nullptr || !condition->validate(senderId, value))
            return false;
    }
    return true;
}


void ConditionsManager::loadFromJson(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        qWarning() << "Could not open JSON file: " << filename.c_str();
        return; 

    nlohmann::json j;
    file >> j;
    file.close();

    qInfo() << "JSON file ----- ----- ----- ----- -----\n" << j.dump(4) << "\n----- ----- ----- ----- -----\n";

    conditions.clear();
    actions.clear();

    addCondition(ConditionsFactory().createConditionsFromJson(j["conditions"]));
    for (const auto& actionJson : j["actions"]) {
        std::string id = actionJson["id"];  
        Action action(actionJson["target"], actionJson["message"]);  
        addAction(id, action);  
    }
}
