#include "doctest.h"
#include "../include/AndCondition.h"
#include "../include/OrCondition.h"
#include <memory>

class TrueCondition : public ConditionBase {
public:
    bool validate() override {
        return true;
    }
    nlohmann::json toJson() const override {
        return { {"type", "true"} };
    }
};

class FalseCondition : public ConditionBase {
public:
    bool validate() override {
        return false;
    }
    nlohmann::json toJson() const override {
        return { {"type", "false"} };
    }
};

// Tests for AndCondition
TEST_CASE("AndCondition Tests") {
    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<TrueCondition>();
        auto trueCond2 = std::make_shared<TrueCondition>();
        AndCondition andCond(trueCond1, trueCond2);
        CHECK(andCond.validate());
        CHECK(andCond.toJson() == nlohmann::json({
            {"type", "AndCondition"},
            {"LHS", nlohmann::json({{"type", "true"}})},
            {"RHS", nlohmann::json({{"type", "true"}})}
            }));
    }

    SUBCASE("One Condition False") {
        auto trueCond = std::make_shared<TrueCondition>();
        auto falseCond = std::make_shared<FalseCondition>();
        AndCondition andCond(trueCond, falseCond);
        CHECK_FALSE(andCond.validate());
        CHECK(andCond.toJson() == nlohmann::json({
            {"type", "AndCondition"},
            {"LHS", nlohmann::json({{"type", "true"}})},
            {"RHS", nlohmann::json({{"type", "false"}})}
            }));
    }
}

// Tests for OrCondition
TEST_CASE("OrCondition Tests") {
    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<TrueCondition>();
        auto trueCond2 = std::make_shared<TrueCondition>();
        OrCondition orCond(trueCond1, trueCond2);
        CHECK(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "OrCondition"},
            {"LHS", nlohmann::json({{"type", "true"}})},
            {"RHS", nlohmann::json({{"type", "true"}})}
            }));
    }

    SUBCASE("One Condition True") {
        auto trueCond = std::make_shared<TrueCondition>();
        auto falseCond = std::make_shared<FalseCondition>();
        OrCondition orCond(trueCond, falseCond);
        CHECK(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "OrCondition"},
            {"LHS", nlohmann::json({{"type", "true"}})},
            {"RHS", nlohmann::json({{"type", "false"}})}
            }));
    }

    SUBCASE("Both Conditions False") {
        auto falseCond1 = std::make_shared<FalseCondition>();
        auto falseCond2 = std::make_shared<FalseCondition>();
        OrCondition orCond(falseCond1, falseCond2);
        CHECK_FALSE(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "OrCondition"},
            {"LHS", nlohmann::json({{"type", "false"}})},
            {"RHS", nlohmann::json({{"type", "false"}})}
            }));
    }
}
