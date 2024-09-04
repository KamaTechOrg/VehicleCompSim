#include "Action.h"

Action::Action(unsigned targetUnit, std::string messageToSend)
	: _targetUnit(targetUnit), _messageToSend(messageToSend) {}

nlohmann::json Action::toJson() const
{
	return {
		{"target", _targetUnit},
		{"message", _messageToSend}
	};
}
