#ifndef ACTION_H
#define ACTION_H

#include <string>

#include "json.hpp"

// This class represents an Action that will take place
// if one of the conditions is met
class Action
{
public:
	Action(std::string targetUnit, std::string messageToSend);
	Action(const Action& other);
	Action() = default;
	~Action() = default;

	std::string getTargetUnit() const;
	std::string getMessageToSend() const;
	nlohmann::json toJson() const;

private:
	std::string _targetUnit;
	std::string _messageToSend;
};

#endif // !ACTION_H
