#include "ConditionsFactory.h"

std::shared_ptr<ConditionBase> ConditionsFactory::createSimpleCondition(
	std::string input, std::string conditionType, std::string validationValue)
{
	auto it = _simpleConditionsMap.find(conditionType);
	if (it != _simpleConditionsMap.end()) {
		return it->second(input, validationValue);
	}
	else {
		return nullptr;
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
	if (j["type"] == "AndCondition" || j["type"] == "OrCondition") {
		return createCompositeCondition(j["type"], createConditionsFromJson(j["lhs"]), createConditionsFromJson(j["rhs"]));
	}
	else {
		return (createSimpleCondition(j["senderId"], j["type"], j["validationValue"]));
	}
}

std::vector<std::string> ConditionsFactory::getSimpleConditionTypes()
{
	std::vector<std::string> types;

	for (const auto& pair : _simpleConditionsMap)
		types.push_back(pair.first);

	return types;
}