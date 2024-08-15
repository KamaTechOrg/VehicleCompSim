#ifndef CONTAINSCondition_H
#define CONTAINSCondition_H


#include "SimpleCondition.h"

class ContainsCondition : public SimpleCondition {
public:
    ContainsCondition(const std::string &input, const std::string &validationValue);
    bool validate(const std::string &senderId, const std::string &value) const override;
    nlohmann::json toJson() const override;
};

#endif 