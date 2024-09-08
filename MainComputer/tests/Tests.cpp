#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS 

#include <memory>
#include <chrono>

#include "doctest.h"

#include "EqualsToCondition.h"
#include "AndCondition.h"
#include "OrCondition.h"
#include "SimpleCondition.h"
#include "ContainsCondition.h"
#include "GreaterThanCondition.h"
#include "SmallerThanCondition.h"
#include "StartsWithCondition.h"
#include "EndsWithCondition.h"
#include "ConditionsFactory.h"

TEST_CASE("SimpleCondition Tests") {
    const std::string senderId = "testSender";

    SUBCASE("Equals To Validation") {
        EqualsToCondition cond(senderId, "value");
        CHECK(cond.validate(senderId, "value"));
        CHECK_FALSE(cond.validate(senderId, "other"));
        CHECK_FALSE(cond.validate("wrongSender", "value"));
    }

    SUBCASE("Greater Than Validation") {
        GreaterThanCondition cond(senderId, "5");
        CHECK(cond.validate(senderId, "10"));
        CHECK_FALSE(cond.validate(senderId, "3"));
        CHECK_FALSE(cond.validate("wrongSender", "10"));
    }

    SUBCASE("Smaller Than Validation") {
        SmallerThanCondition cond(senderId, "5");
        CHECK(cond.validate(senderId, "3"));
        CHECK_FALSE(cond.validate(senderId, "10"));
        CHECK_FALSE(cond.validate("wrongSender", "3"));
    }

    SUBCASE("Starts With Validation") {
        StartsWithCondition cond(senderId, "hello");
        CHECK(cond.validate(senderId, "hello world"));
        CHECK_FALSE(cond.validate(senderId, "world hello"));
        CHECK_FALSE(cond.validate("wrongSender", "hello world"));
    }

    SUBCASE("Ends With Validation") {
        EndsWithCondition cond(senderId, "world");
        CHECK(cond.validate(senderId, "hello world"));
        CHECK_FALSE(cond.validate(senderId, "world hello"));
        CHECK_FALSE(cond.validate("wrongSender", "hello world"));
    }

    SUBCASE("Contains Validation") {
        ContainsCondition cond(senderId, "lo wo");
        CHECK(cond.validate(senderId, "hello world"));
        CHECK_FALSE(cond.validate(senderId, "hi there"));
        CHECK_FALSE(cond.validate("wrongSender", "hello world"));
    }
}

TEST_CASE("AndCondition Tests") {
    const std::string senderId = "testSender";
    const std::chrono::milliseconds elapsedTime(100);

    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<StartsWithCondition>(senderId, "val");
        auto trueCond2 = std::make_shared<EndsWithCondition>(senderId, "ue");
        AndCondition andCond(trueCond1, trueCond2, elapsedTime);
        CHECK(andCond.validate(senderId, "value"));
        CHECK_FALSE(andCond.validate("wrongSender", "value"));

        nlohmann::json expectedJson = {
            {"type", "AndCondition"},
            {"lhs", trueCond1->toJson()},
            {"rhs", trueCond2->toJson()},
            {"elapsedTime", elapsedTime.count()}
        };
        CHECK(andCond.toJson() == expectedJson);
    }

    SUBCASE("One Condition False") {
        auto trueCond = std::make_shared<StartsWithCondition>(senderId, "val");
        auto falseCond = std::make_shared<EndsWithCondition>(senderId, "world");
        AndCondition andCond(trueCond, falseCond, elapsedTime);
        CHECK_FALSE(andCond.validate(senderId, "value"));
    }
}

TEST_CASE("OrCondition Tests") {
    const std::string senderId = "testSender";
    const std::chrono::milliseconds elapsedTime(100);

    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<StartsWithCondition>(senderId, "hello");
        auto trueCond2 = std::make_shared<EndsWithCondition>(senderId, "world");
        OrCondition orCond(trueCond1, trueCond2, elapsedTime);
        CHECK(orCond.validate(senderId, "hello world"));
        CHECK_FALSE(orCond.validate("wrongSender", "hello world"));

        nlohmann::json expectedJson = {
            {"type", "OrCondition"},
            {"lhs", trueCond1->toJson()},
            {"rhs", trueCond2->toJson()},
            {"elapsedTime", elapsedTime.count()}
        };
        CHECK(orCond.toJson() == expectedJson);
    }

    SUBCASE("One Condition True") {
        auto trueCond = std::make_shared<StartsWithCondition>(senderId, "hello");
        auto falseCond = std::make_shared<EndsWithCondition>(senderId, "unknown");
        OrCondition orCond(trueCond, falseCond, elapsedTime);
        CHECK(orCond.validate(senderId, "hello world"));
    }

    SUBCASE("Both Conditions False") {
        auto falseCond1 = std::make_shared<StartsWithCondition>(senderId, "world");
        auto falseCond2 = std::make_shared<EndsWithCondition>(senderId, "hello");
        OrCondition orCond(falseCond1, falseCond2, elapsedTime);
        CHECK_FALSE(orCond.validate(senderId, "hello world"));
    }
}

TEST_CASE("SimpleCondition Edge Cases") {
    const std::string senderId = "testSender";

    SUBCASE("Empty string comparison") {
        EqualsToCondition cond(senderId, "");
        CHECK(cond.validate(senderId, ""));
    }

    SUBCASE("Case sensitivity") {
        EqualsToCondition cond(senderId, "Value");
        CHECK_FALSE(cond.validate(senderId, "value"));
    }

    SUBCASE("Whitespace handling") {
        EqualsToCondition cond(senderId, "value");
        CHECK_FALSE(cond.validate(senderId, " value "));
    }

    SUBCASE("Contains with empty string") {
        ContainsCondition cond(senderId, "");
        CHECK(cond.validate(senderId, "hello world"));
    }

    SUBCASE("Contains with full string") {
        ContainsCondition cond(senderId, "hello world");
        CHECK(cond.validate(senderId, "hello world"));
    }
}

TEST_CASE("Nested Composite Conditions") {
    const std::string senderId = "testSender";
    const std::chrono::milliseconds elapsedTime(100);

    auto cond1 = std::make_shared<StartsWithCondition>(senderId, "v");
    auto cond2 = std::make_shared<ContainsCondition>(senderId, "alu");
    auto cond3 = std::make_shared<EndsWithCondition>(senderId, "e");

    SUBCASE("Nested And-Or") {
        auto andCond = std::make_shared<AndCondition>(cond1, cond2, elapsedTime);
        OrCondition orCond(andCond, cond3, elapsedTime);
        CHECK(orCond.validate(senderId, "value"));
    }
}

TEST_CASE("Performance Test") {
    const std::string senderId = "testSender";
    const std::chrono::milliseconds elapsedTime(1);
    const int NUM_OF_CONDITIONS = 1000;
    std::vector<std::shared_ptr<ConditionBase>> conditions;

    for (int i = 0; i < NUM_OF_CONDITIONS; ++i) {
        conditions.push_back(std::make_shared<ContainsCondition>(
            senderId, std::to_string(i)));
    }

    auto andCond = std::make_shared<AndCondition>(
        std::make_shared<StartsWithCondition>(senderId, "hello"),
        std::make_shared<EndsWithCondition>(senderId, "world"),
        elapsedTime
    );
    auto complexCondition = std::make_shared<OrCondition>(
        andCond,
        std::make_shared<EqualsToCondition>(senderId, "test"),
        elapsedTime
    );

    SUBCASE("Performance Measurement") {
        auto start = std::chrono::high_resolution_clock::now();
        bool result = complexCondition->validate(senderId, "hello test world");
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        CHECK(duration.count() < 1000);
        CHECK(result);
    }
}

TEST_CASE("ConditionsFactory Test") {
    /*ConditionsFactory conditionsFactory;

    std::vector<std::string> conditionsTypes = conditionsFactory.getSimpleConditionTypes();

    for (const auto&type : conditionsTypes) {
        std::shared_ptr<ConditionBase> condition = conditionsFactory.createSimpleCondition("id", type, "value");
    }*/
}

TEST_CASE("Simple Conditions ToJson Test") {

}