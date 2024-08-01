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
        auto trueCond1 = std::make_unique<TrueCondition>();
        auto trueCond2 = std::make_unique<TrueCondition>();
        AndCondition andCond(std::move(trueCond1), std::move(trueCond2));
        CHECK(andCond.validate());
        CHECK(andCond.toJson() == nlohmann::json({
            {"type", "and"},
            {"left", nlohmann::json({{"type", "true"}})},
            {"right", nlohmann::json({{"type", "true"}})}
            }));
    }

    SUBCASE("One Condition False") {
        auto trueCond = std::make_unique<TrueCondition>();
        auto falseCond = std::make_unique<FalseCondition>();
        AndCondition andCond(std::move(trueCond), std::move(falseCond));
        CHECK_FALSE(andCond.validate());
        CHECK(andCond.toJson() == nlohmann::json({
            {"type", "and"},
            {"left", nlohmann::json({{"type", "true"}})},
            {"right", nlohmann::json({{"type", "false"}})}
            }));
    }
}

// Tests for OrCondition
TEST_CASE("OrCondition Tests") {
    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_unique<TrueCondition>();
        auto trueCond2 = std::make_unique<TrueCondition>();
        OrCondition orCond(std::move(trueCond1), std::move(trueCond2));
        CHECK(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "or"},
            {"left", nlohmann::json({{"type", "true"}})},
            {"right", nlohmann::json({{"type", "true"}})}
            }));
    }

    SUBCASE("One Condition True") {
        auto trueCond = std::make_unique<TrueCondition>();
        auto falseCond = std::make_unique<FalseCondition>();
        OrCondition orCond(std::move(trueCond), std::move(falseCond));
        CHECK(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "or"},
            {"left", nlohmann::json({{"type", "true"}})},
            {"right", nlohmann::json({{"type", "false"}})}
            }));
    }

    SUBCASE("Both Conditions False") {
        auto falseCond1 = std::make_unique<FalseCondition>();
        auto falseCond2 = std::make_unique<FalseCondition>();
        OrCondition orCond(std::move(falseCond1), std::move(falseCond2));
        CHECK_FALSE(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "or"},
            {"left", nlohmann::json({{"type", "false"}})},
            {"right", nlohmann::json({{"type", "false"}})}
            }));
    }
}
