#ifndef ACTION_H
#define ACTION_H

#include <string>

#include "json.hpp"

class Action
{
public:
	Action(unsigned targetUnit, std::string messageToSend);
	Action() = default;
	~Action() = default;

	unsigned getTargetUnit() const;     
	std::string getMessageToSend() const;
	nlohmann::json toJson() const;

private:
	unsigned _targetUnit;
	std::string _messageToSend;
};

#endif // !ACTION_H
