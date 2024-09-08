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
		return nullptr;
	}
}

std::shared_ptr<ConditionBase> ConditionsFactory::createConditionsFromJson(nlohmann::json j)
{
	try {
		if (j["type"] == "And" || j["type"] == "Or") {
			return createCompositeCondition(j["type"], createConditionsFromJson(j["lhs"]), createConditionsFromJson(j["rhs"]));
		}
		else {
			return (createSimpleCondition(j["senderId"], j["type"], j["validationValue"]));
		}
	}
	catch (const std::exception& e) {
		qWarning() << e.what();
	}
}

std::vector<std::string> ConditionsFactory::getSimpleConditionTypes()
{
	std::vector<std::string> types;

	for (const auto& pair : _simpleConditionsMap)
		types.push_back(pair.first);

	return types;
}