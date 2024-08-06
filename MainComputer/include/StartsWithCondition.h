#ifndef STARTS_WITH_CONDITION_H
#define STARTS_WITH_CONDITION_H

#include "SimpleCondition.h"

class StartsWithCondition : public SimpleCondition {
public:
    StartsWithCondition(const std::string& senderId, const std::string& validationValue);
    bool validate(const std::string& senderId, const std::string& value) const override;
    nlohmann::json toJson() const override;
};

#endif
