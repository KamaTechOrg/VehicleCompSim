#include "ConditionsManager.h"
#include <stdexcept>
#include <fstream>

void ConditionsManager::addCondition(std::shared_ptr<ConditionBase> condition) {
    conditions.push_back(condition);
}

bool ConditionsManager::validateAll() const {
    for (const auto& condition : conditions) {
        if (!condition->validate()) {
            return false;
        }
    }
    return true;
}

void ConditionsManager::exportToJson(const std::string& filename) const {
    nlohmann::json j;
    for (const auto& condition : conditions) {
        j["conditions"].push_back(condition->toJson());
    }
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file << j.dump(4);
}
