#include "ConditionsFactory.h"

#include "GreaterThanCondition.h"
#include "SmallerThanCondition.h"
#include "EqualsToCondition.h"
#include "StartsWithCondition.h"
#include "EndsWithCondition.h"
#include "ContainsCondition.h"

std::shared_ptr<ConditionBase> ConditionsFactory::createCondition(std::string input, std::string conditionType, std::string validationValue)
{
	static const std::unordered_map<std::string, std::function<std::shared_ptr<ConditionBase>(std::string input, std::string validationValue)>> conditionFactory = {
		{"greater than", [](std::string input, std::string validationValue) {
			return std::make_shared<GreaterThanCondition>(input, validationValue);
		}},
		{"smaller than", [](std::string input, std::string validationValue) {
			return std::make_shared<SmallerThanCondition>(input, validationValue);
		}},
		{"equals to", [](std::string input, std::string validationValue) {
			return std::make_shared<EqualsToCondition>(input, validationValue);
		}},
		{"starts with", [](std::string input, std::string validationValue) {
			return std::make_shared<StartsWithCondition>(input, validationValue);
		}},
		{"ends with", [](std::string input, std::string validationValue) {
			return std::make_shared<EndsWithCondition>(input, validationValue);
		}},
		{"contains", [](std::string input, std::string validationValue) {
			return std::make_shared<ContainsCondition>(input, validationValue);
		}}
	};

	auto it = conditionFactory.find(conditionType);
	if (it != conditionFactory.end()) {
		return it->second(input, validationValue);
	}
	else {
		return nullptr;
	}
}
