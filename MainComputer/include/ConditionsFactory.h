#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

#include "ConditionBase.h"
#include "GreaterThanCondition.h"
#include "SmallerThanCondition.h"
#include "EqualsToCondition.h"
#include "StartsWithCondition.h"
#include "EndsWithCondition.h"
#include "ContainsCondition.h"


class ConditionsFactory
{
public:
	std::shared_ptr<ConditionBase> createCondition(std::string input, std::string conditionType, std::string validationValue);

private:
    const std::unordered_map<std::string, std::function<std::shared_ptr<ConditionBase>(const std::string&, const std::string&)>> _conditionsMap = {
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
};