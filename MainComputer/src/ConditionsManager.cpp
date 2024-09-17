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
std::vector<std::vector<Action>> ConditionsManager::actions;

ConditionsManager::ConditionsManager()
    : _isRunning(false)
{
    loadFromJson(constants::CONDITIONS_JSON_FILE_NAME);
}

void ConditionsManager::addAction(const int index, const Action &action)
{
    actions[index].push_back(action);
}


void ConditionsManager::executeActions(const int index)
{
    if (actions.size() < index || actions.at(index).empty()) {
        qWarning() << "No action found for index: " << index;
        return;
    }

    Communication communication;
    for (const auto& action : actions.at(index)) {
        int targetUnit = std::stoi(action.getTargetUnit());
        communication.sendTo(targetUnit, action.getMessageToSend());
        qInfo() << "Action executed: Sent message to index: " << index;
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
                    executeActions(0);
                    /*
                    * TODO: executeActions should execute the actions for the indices
                    *       that were actualy validated as "true".
                    *       (on the validation loop at "validateAll" if it was "true" at
                    *       index i, then we execute all actions at index i).
                    */
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
    // add an entry to the actions that will be executed
    // if this added condition will be validated as "true"
    actions.push_back(std::vector<Action>(0));
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
    {
        qWarning() << "Could not open JSON file: " << filename.c_str();
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    qInfo() << "JSON file ----- ----- ----- ----- -----\n" << j.dump(4) << "\n----- ----- ----- ----- -----\n";

    conditions.clear();
    actions.clear();

    addCondition(ConditionsFactory().createConditionsFromJson(j["conditions"]));

    for (const auto& actionJson : j["actions"]) {
            std::string target = actionJson["target"];
            std::string message = actionJson["message"];
            Action action(target, message);
            addAction(0, action);
        }
    }

