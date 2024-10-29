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

#include "JsonLoader.h"


std::vector<std::shared_ptr<ConditionBase>> ConditionsManager::conditions;
std::vector<std::vector<Action>> ConditionsManager::actions;

ConditionsManager::ConditionsManager()
    : _isRunning(false)
{
    loadFromJson();
}

void ConditionsManager::addAction(const int index, const Action& action)
{
    actions[index].push_back(action);
}

void ConditionsManager::executeActions(const int index) const
{
    if (index >= actions.size() || actions.at(index).empty()) {
        qWarning() << "No action found for index:" << index;
        return;
    }

    Communication communication;
    for (const auto& action : actions.at(index)) {
        int targetUnit = std::stoi(action.getTargetUnit());
        communication.sendTo(targetUnit, action.getMessageToSend());
        qInfo() << "Action executed: Sent message to index:" << index;
    }
}

std::pair<std::string, std::string> ConditionsManager::parseMessage(const std::string& message)
{
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

        communication.sendAndReceiveLoop("172.232.208.10", 8080);
        communication.sendTo(8080, "Hello, Server!");
        communication.connectToSensors();

        while (_isRunning)
        {
            qInfo() << "Running";
            std::string message = communication.getMessageFromQueue();
            qInfo() << "Message received from sensor:" << QString::fromStdString(message);

            try {
                auto [id, value] = parseMessage(message);
                qInfo() << "Parsed message - ID:" << QString::fromStdString(id) << ", VALUE:" << QString::fromStdString(value);
                validateAll(id, value);
            }
            catch (const std::exception& e) {
                qWarning() << "Failed to parse message:" << e.what();
            }
        }
        qInfo() << "Conditions Manager thread stopping";
        }).detach();
}

void ConditionsManager::stop()
{
    _isRunning = false;
    qInfo() << "Stopping Main Computer" << "\n----- ----- ----- ----- -----\n";
}

bool ConditionsManager::isRunning()
{
    qInfo() << "Running Main Computer" << "\n----- ----- ----- ----- -----\n";
    return _isRunning;
}

void ConditionsManager::addCondition(std::shared_ptr<ConditionBase> condition)
{
    if (condition == nullptr) return;

    conditions.push_back(condition);
    actions.push_back(std::vector<Action>(0));
}

void ConditionsManager::validateAll(const std::string& senderId, const std::string& value) const
{
    bool anyValidationSucceeded = false;
    for (int i = 0; i < conditions.size(); i++)
    {
        if (conditions.at(i) == nullptr)
            continue;

        if (conditions.at(i)->validate(senderId, value))
        {
            qInfo() << "Validation succeeded for ID:" << QString::fromStdString(senderId) << " with value:" << QString::fromStdString(value);
            executeActions(i);
            anyValidationSucceeded = true;
        }
    }

    if (!anyValidationSucceeded)
    {
        qInfo() << "No conditions matched for ID:" << QString::fromStdString(senderId) << " with value:" << QString::fromStdString(value);
    }
}

void ConditionsManager::loadFromJson()
{
    nlohmann::json jsonData = JsonLoader().loadConditionsLogic();
    conditions.clear();
    actions.clear();

    if (!jsonData.is_array() || jsonData.empty())
        return;

    for (int i = 0; i < jsonData.size(); i++)
    {
        if (!jsonData.at(i).contains("conditions") || !jsonData.at(i).contains("actions"))
            continue;

        addCondition(ConditionsFactory().createConditionsFromJson(jsonData.at(i)["conditions"]));
        for (const auto& actionJson : jsonData.at(i)["actions"]) {
            std::string target = actionJson["target"];
            std::string message = actionJson["message"];
            Action action(target, message);
            addAction(i, action);
        }
    }
}
