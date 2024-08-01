#include "OrCondition.h"

bool OrCondition::validate() {
    return LHS->validate() || RHS->validate();
}

nlohmann::json OrCondition::toJson() const {
    return {
        {"type", "or"},
        {"left", LHS->toJson()},
        {"right", RHS->toJson()}
    };
}
