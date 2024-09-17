#include "ConditionsFactory.h"

#include <exception>
#include <QDebug>

std::shared_ptr<ConditionBase> ConditionsFactory::createSimpleCondition(
	std::string senderID, std::string conditionType, std::string validationValue)
{
	auto it = _simpleConditionsMap.find(conditionType);
	if (it != _simpleConditionsMap.end()) {
		return it->second(senderID, validationValue);
	}
	else {
		throw std::invalid_argument("condition type: " + conditionType + " is not an option");
	}
}

std::shared_ptr<ConditionBase> ConditionsFactory::createCompositeCondition(const std::string& conditionType, const std::shared_ptr<ConditionBase>& lhs, const std::shared_ptr<ConditionBase>& rhs)
{
	auto it = _compositeConditionsMap.find(conditionType);
	if (it != _compositeConditionsMap.end()) {
		return it->second(lhs, rhs);
	}
	else {
		throw std::invalid_argument("condition type: " + conditionType + " is not an option");
	}
}

std::shared_ptr<ConditionBase> ConditionsFactory::createConditionsFromJson(nlohmann::json jsonData)
{
    try {
        if (jsonData.is_null() || jsonData.empty()) {
            qWarning() << "JSON data is empty";
            return nullptr;
        }

        if (!jsonData.contains("type")) {
            qWarning() << "JSON data does not contain 'type' key";
            return nullptr;
        }

        if (jsonData["type"] == "And" || jsonData["type"] == "Or") {
            return createCompositeCondition(
                jsonData["type"],
                createConditionsFromJson(jsonData["lhs"]),
                createConditionsFromJson(jsonData["rhs"])
            );
        }
        else {
            return createSimpleCondition(
                jsonData["senderId"],
                jsonData["type"],
                jsonData["validationValue"]
            );
        }
    }
    catch (const std::exception& e) {
        qWarning() << e.what();
        return nullptr;  // Return a nullptr if an error occurs
    }
}

std::vector<std::string> ConditionsFactory::getSimpleConditionTypes()
{
	std::vector<std::string> types;

	for (const auto& pair : _simpleConditionsMap)
		types.push_back(pair.first);

	return types;
}

std::vector<std::string> ConditionsFactory::getCompositeConditionTypes()
{
	std::vector<std::string> types;

	for (const auto& pair : _compositeConditionsMap)
		types.push_back(pair.first);

	return types;
}
