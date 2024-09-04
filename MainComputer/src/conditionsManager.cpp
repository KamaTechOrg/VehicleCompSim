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

// Helper function to parse messages
std::pair<std::string, std::string> ConditionsManager::parseMessage(const std::string &message)
{
    // Assuming the message format is "ID:<ID_VALUE>,VALUE:<VALUE>"
    std::regex messageRegex(R"(ID:(\w+),VALUE:(\w+))");
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
        std::string count;
        while (_isRunning)
        {
            count.push_back('.');
            qInfo() << "running " << count.c_str();

            if (count.size() == 10)
                count.clear();

            // Sleep for 1 second to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            /*
           * TODO: listen to camera/sensors and validateAll() for each message.
           */

            const int SENSOR_PORT_NUMBER = 8101;
            Communication communication;
            communication.connectTo(SENSOR_PORT_NUMBER);

            std::string message = "ID:some_id,VALUE:some_value";

            try {
                auto [id, value] = parseMessage(message);

                if (validateAll(id, value)) {
                    qInfo() << "Validation succeeded for ID:" << id.c_str() << " with value:" << value.c_str();
                    executeAction(id);  // Execute the associated action
                }
                else {
                    qWarning() << "Validation failed for ID:" << id.c_str() << " with value:" << value.c_str();
                }
            }
            catch (const std::exception& e) {
                qWarning() << "Failed to parse message:" << e.what();
            }
        }
        }).detach(); // Detach the thread so it runs independently 

        //const int PORT_NUMBER = 8101;
    //Communication().sendTo(PORT_NUMBER, "Hello From Main Computer!");

    //Communication().sendTo(PORT_NUMBER, "BEEP 2000 100 128");
}

void ConditionsManager::executeAction(const std::string &id)
{
    auto it = actions.find(id);
    if (it != actions.end()) {
        const Action& action = it->second;
        Communication communication;
        communication.sendTo(action.getTargetUnit(), action.getMessageToSend());
        qInfo() << "Action executed: Sent message to ID:" << id.c_str();
    }
    else {
        qWarning() << "No action found for ID:" << id.c_str();
    }
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

bool ConditionsManager::validateAll(const std::string &senderId, const std::string &value) const
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
        return;  // TODO: maybe throw some error, but for now it's not handled anyway

    nlohmann::json j;
    file >> j;
    file.close();

    qInfo() << "JSON file ----- ----- ----- ----- -----\n" << j.dump(4) << "\n----- ----- ----- ----- -----\n";

    conditions.clear();
    actions.clear(); 

    addCondition(ConditionsFactory().createConditionsFromJson(j["conditions"]));

    for (const auto& actionJson : j["actions"]) {
        Action action(actionJson["target"], actionJson["message"]);
        addAction(actionJson["id"], action);
    }
}
