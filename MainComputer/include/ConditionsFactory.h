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
#include "AndCondition.h"
#include "OrCondition.h"


class ConditionsFactory
{
public:
	std::shared_ptr<ConditionBase> createSimpleCondition(std::string input, std::string conditionType, std::string validationValue);
	std::shared_ptr<ConditionBase> createCompositeCondition(const std::string& conditionType, const std::shared_ptr<ConditionBase>& lhs, const std::shared_ptr<ConditionBase>& rhs);
	std::shared_ptr<ConditionBase> createConditionsFromJson(nlohmann::json j);

	std::vector<std::string> getSimpleConditionTypes();

private:
	inline static const std::unordered_map<std::string, std::function<std::shared_ptr<ConditionBase>(const std::string&, const std::string&)>> _simpleConditionsMap = {
		{"greater than", [](const std::string& input, const std::string& validationValue) {
			return std::make_shared<GreaterThanCondition>(input, validationValue);
		}},
		{"smaller than", [](const std::string& input, const std::string& validationValue) {
			return std::make_shared<SmallerThanCondition>(input, validationValue);
		}},
		{"equals to", [](const std::string& input, const std::string& validationValue) {
			return std::make_shared<EqualsToCondition>(input, validationValue);
		}},
		{"starts with", [](const std::string& input, const std::string& validationValue) {
			return std::make_shared<StartsWithCondition>(input, validationValue);
		}},
		{"ends with", [](const std::string& input, const std::string& validationValue) {
			return std::make_shared<EndsWithCondition>(input, validationValue);
		}},
		{"contains", [](const std::string& input, const std::string& validationValue) {
			return std::make_shared<ContainsCondition>(input, validationValue);
		}}
	};

	inline static const std::unordered_map<std::string, std::function<std::shared_ptr<ConditionBase>(const std::shared_ptr<ConditionBase>&, const std::shared_ptr<ConditionBase>&)>> _compositeConditionsMap = {
		{"AndCondition", [](const std::shared_ptr<ConditionBase>& lhs, const std::shared_ptr<ConditionBase>& rhs) {
			return std::make_shared<AndCondition>(lhs, rhs);
		}},
		{"AndCondition", [](const std::shared_ptr<ConditionBase>& lhs, const std::shared_ptr<ConditionBase>& rhs) {
			return std::make_shared<OrCondition>(lhs, rhs);
		}}
	};
};