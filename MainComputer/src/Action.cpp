#include "Action.h"

Action::Action(unsigned targetUnit, std::string messageToSend)
    : _targetUnit(targetUnit), _messageToSend(messageToSend) {}

unsigned Action::getTargetUnit() const {
    return _targetUnit;
}

std::string Action::getMessageToSend() const {
    return _messageToSend;
}

nlohmann::json Action::toJson() const {
    return {
        {"target", _targetUnit},
        {"message", _messageToSend}
    };
}
