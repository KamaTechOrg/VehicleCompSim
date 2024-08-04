#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS 
#include "doctest.h"
#include "../include/AndCondition.h"
#include "../include/OrCondition.h"
#include "../include/SimpleCondition.h"
#include <memory>
#include <chrono>

class TrueCondition : public ConditionBase {
public:
    bool validate() override { return true; }
    nlohmann::json toJson() const override { return { {"type", "true"} }; }
};

class FalseCondition : public ConditionBase {
public:
    bool validate() override { return false; }
    nlohmann::json toJson() const override { return { {"type", "false"} }; }
};

TEST_CASE("SimpleCondition Tests") {
    SUBCASE("Equals To Validation") {
        SimpleCondition cond("value", "equals to", "value");
        CHECK(cond.validate());
    }

    SUBCASE("Greater Than Validation") {
        SimpleCondition cond("10", "greater than", "5");
        CHECK(cond.validate());
    }

    SUBCASE("Smaller Than Validation") {
        SimpleCondition cond("3", "smaller than", "5");
        CHECK(cond.validate());
    }

    SUBCASE("Starts With Validation") {
        SimpleCondition cond("hello world", "starts with", "hello");
        CHECK(cond.validate());
    }

    SUBCASE("Ends With Validation") {
        SimpleCondition cond("hello world", "ends with", "world");
        CHECK(cond.validate());
    }

    SUBCASE("Contains Validation") {
        SimpleCondition cond("hello world", "contains", "lo wo");
        CHECK(cond.validate());
    }

    SUBCASE("Does Not Contain Validation") {
        SimpleCondition cond("hello world", "contains", "goodbye");
        CHECK_FALSE(cond.validate());
    }
}

TEST_CASE("AndCondition Tests") {
    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<SimpleCondition>("value", "starts with", "val");
        auto trueCond2 = std::make_shared<SimpleCondition>("value", "ends with", "ue");
        AndCondition andCond(trueCond1, trueCond2);
        CHECK(andCond.validate());
    }

    SUBCASE("One Condition False") {
        auto trueCond = std::make_shared<SimpleCondition>("value", "starts with", "val");
        auto falseCond = std::make_shared<SimpleCondition>("value", "ends with", "world");
        AndCondition andCond(trueCond, falseCond);
        CHECK_FALSE(andCond.validate());
    }

    SUBCASE("Contains and Starts With") {
        auto containsCond = std::make_shared<SimpleCondition>("hello world", "contains", "lo");
        auto startsCond = std::make_shared<SimpleCondition>("hello world", "starts with", "hello");
        AndCondition andCond(containsCond, startsCond);
        CHECK(andCond.validate());
    }
}

TEST_CASE("OrCondition Tests") {
    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<SimpleCondition>("hello world", "starts with", "hello");
        auto trueCond2 = std::make_shared<SimpleCondition>("hello world", "ends with", "world");
        OrCondition orCond(trueCond1, trueCond2);
        CHECK(orCond.validate());
    }

    SUBCASE("One Condition True") {
        auto trueCond = std::make_shared<SimpleCondition>("hello world", "starts with", "hello");
        auto falseCond = std::make_shared<SimpleCondition>("hello world", "ends with", "unknown");
        OrCondition orCond(trueCond, falseCond);
        CHECK(orCond.validate());
    }

    SUBCASE("Contains or Equals To") {
        auto containsCond = std::make_shared<SimpleCondition>("hello world", "contains", "universe");
        auto equalsCond = std::make_shared<SimpleCondition>("hello world", "equals to", "hello world");
        OrCondition orCond(containsCond, equalsCond);
        CHECK(orCond.validate());
    }
}

TEST_CASE("SimpleCondition Edge Cases") {
    SUBCASE("Empty string comparison") {
        SimpleCondition cond("", "equals to", "");
        CHECK(cond.validate());
    }

    SUBCASE("Case sensitivity") {
        SimpleCondition cond("Value", "equals to", "value");
        CHECK_FALSE(cond.validate());
    }

    SUBCASE("Whitespace handling") {
        SimpleCondition cond(" value ", "equals to", "value");
        CHECK_FALSE(cond.validate());
    }

    SUBCASE("Contains with empty string") {
        SimpleCondition cond("hello world", "contains", "");
        CHECK(cond.validate());
    }

    SUBCASE("Contains with full string") {
        SimpleCondition cond("hello world", "contains", "hello world");
        CHECK(cond.validate());
    }
}

TEST_CASE("Nested Composite Conditions") {
    auto cond1 = std::make_shared<SimpleCondition>("value", "starts with", "v");
    auto cond2 = std::make_shared<SimpleCondition>("value", "contains", "alu");
    auto cond3 = std::make_shared<SimpleCondition>("value", "ends with", "e");

    SUBCASE("Nested And-Or with Contains") {
        auto andCond = std::make_shared<AndCondition>(cond1, cond2);
        OrCondition orCond(andCond, cond3);
        CHECK(orCond.validate());
    }
}

TEST_CASE("Exception Handling") {
    SUBCASE("Invalid validation type") {
        SimpleCondition cond("value", "invalid type", "value");
        CHECK_THROWS_AS(cond.validate(), std::runtime_error);
    }
}

TEST_CASE("Performance Test") {
    const int NUM_CONDITIONS = 1000;
    std::vector<std::shared_ptr<ConditionBase>> conditions;

    for (int i = 0; i < NUM_CONDITIONS; ++i) {
        conditions.push_back(std::make_shared<SimpleCondition>(
            "test string " + std::to_string(i), "contains", std::to_string(i)));
    }

    auto complexCondition = std::make_shared<AndCondition>(conditions[0], conditions[1]);
    for (int i = 2; i < NUM_CONDITIONS; ++i) {
        complexCondition = std::make_shared<AndCondition>(complexCondition, conditions[i]);
    }

    auto start = std::chrono::high_resolution_clock::now();
    bool result = complexCondition->validate();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    CHECK(duration.count() < 1000);
    CHECK(result);
}