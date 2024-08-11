#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

#include "ConditionBase.h"


class ConditionsFactory
{
public:
	std::shared_ptr<ConditionBase> createCondition(std::string input, std::string conditionType, std::string validationValue);

private:

};