#include "ConditionsFactory.h"

std::shared_ptr<ConditionBase> ConditionsFactory::createCondition(
	std::string input, std::string conditionType, std::string validationValue)
{
	auto it = _conditionsMap.find(conditionType);
	if (it != _conditionsMap.end()) {
		return it->second(input, validationValue);
	}
	else {
		return nullptr;
	}
}
