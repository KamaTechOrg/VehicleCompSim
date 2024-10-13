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

void ConditionsManager::addAction(const int index, const Action &action)
{
    actions[index].push_back(action);
}


void ConditionsManager::executeActions(const int index) const
{
    if (actions.size() < index || actions.at(index).empty()) {
        qWarning() << "No action found for index: " << index;
        return;
    }

    Communication communication;
    for (const auto& action : actions.at(index)) {
        std::string message = "TARGET:beep controller," + action.getMessageToSend();
        communication.sendTo(constants::SERVER_PORT, message);
        qInfo() << "Action executed: Sent message to BEEP controller: " << QString::fromStdString(message);
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


void ConditionsManager::sendTargetMessage(const std::string& targetController, const std::string& message) const {
    Communication communication;
    communication.sendTo(constants::SERVER_PORT, "TARGET:" + targetController + ", MESSAGE:" + message);
    qInfo() << "Sent message to target controller: " << QString::fromStdString(targetController);
}


void ConditionsManager::run()
{
    _isRunning = true;
    std::thread([this]() {
        Communication communication;
        communication.sendAndReceiveLoop("172.232.208.10", 8080);
        communication.sendTo(8080, "Hello, Server!");
        communication.sendTo(8080, "TARGET:server,MESSAGE:Main computer connected");

        while (_isRunning)
        {
            qInfo() << "Running main computer";
            std::string message = communication.getMessageFromQueue();
            if (message.empty()) {
                qWarning() << "Received empty message from server"; // Log empty message
                continue; // Skip processing if message is empty
            }
            qInfo() << "Message received from sensor: " << QString::fromStdString(message);  // Print to GUI

            try {
                auto messageContent = parseMessage(message);
                std::string id = messageContent.first;
                std::string value = messageContent.second;
                validateAll(id, value);
            }
            catch (const std::exception& e) {
                qWarning() << "Failed to parse message:" << e.what();

            }
            std::this_thread::sleep_for(std::chrono::seconds(1));

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
    if (condition == nullptr)
        return;

    conditions.push_back(condition);
    // add an entry to the actions that will be executed
    // if this added condition will be validated as "true"
    actions.push_back(std::vector<Action>(0));
}

void ConditionsManager::validateAll(const std::string& senderId, const std::string& value) const
{
    for (int i = 0; i < conditions.size(); i++)
    {
        if (conditions.at(i) == nullptr)
            continue;

        if (conditions.at(i)->validate(senderId, value))
        {
            qInfo() << "Validation succeeded for ID:" << senderId.c_str() << " with value:" << value.c_str();
            executeActions(i);
            sendTargetMessage("beep controller", "MESSAGE:Beep");
        }
        else
        {
            qInfo() << "Validation failed for ID:" << senderId.c_str() << " with value:" << value.c_str();
        }
    }
}


void ConditionsManager::loadFromJson()
{
    nlohmann::json jsonData = JsonLoader().loadConditionsLogic();
    conditions.clear();
    actions.clear();
    if (!jsonData.is_array() || !jsonData.empty())
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