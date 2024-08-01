#include "OrCondition.h"

bool OrCondition::validate() {
    return getLHS()->validate() || getRHS()->validate();
}

nlohmann::json OrCondition::toJson() const {
    return {
        {"type", "OrCondition"},
        {"LHS", getLHS()->toJson()},
        {"RHS", getRHS()->toJson()}
    };
}
