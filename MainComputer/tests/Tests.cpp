#include <memory>
#include <chrono>
#include <cstdio>

#include "doctest.h"

#include "constants.h"
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
#include "JsonLoader.h"

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
            {"type", "And"},
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
            {"type", "Or"},
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

TEST_CASE("Simple Conditions ToJson Test") {
    std::string senderID = "1";
    std::string validationValue = "some_value";

    SUBCASE("Equals To toJson() method") {
        EqualsToCondition cond(senderID, validationValue);

        nlohmann::json expectedJson = {
            {"type", "Equals To"},
            {"senderId", senderID},
            {"validationValue", validationValue}
        };

        CHECK(cond.toJson() == expectedJson);
    }

    SUBCASE("Greater Than toJson() method") {
        GreaterThanCondition cond(senderID, validationValue);

        nlohmann::json expectedJson = {
            {"type", "Greater Than"},
            {"senderId", senderID},
            {"validationValue", validationValue}
        };

        CHECK(cond.toJson() == expectedJson);
    }

    SUBCASE("Smaller Than toJson() method") {
        SmallerThanCondition cond(senderID, validationValue);

        nlohmann::json expectedJson = {
        {"type", "Smaller Than"},
        {"senderId", senderID},
        {"validationValue", validationValue}
        };

        CHECK(cond.toJson() == expectedJson);
    }

    SUBCASE("Starts With toJson() method") {
        StartsWithCondition cond(senderID, validationValue);

        nlohmann::json expectedJson = {
        {"type", "Starts With"},
        {"senderId", senderID},
        {"validationValue", validationValue}
        };

        CHECK(cond.toJson() == expectedJson);
    }

    SUBCASE("Ends With toJson() method") {
        EndsWithCondition cond(senderID, validationValue);

        nlohmann::json expectedJson = {
        {"type", "Ends With"},
        {"senderId", senderID},
        {"validationValue", validationValue}
        };

        CHECK(cond.toJson() == expectedJson);
    }

    SUBCASE("Contains toJson() method") {
        ContainsCondition cond(senderID, validationValue);

        nlohmann::json expectedJson = {
        {"type", "Contains"},
        {"senderId", senderID},
        {"validationValue", validationValue}
        };

        CHECK(cond.toJson() == expectedJson);
    }
}

TEST_CASE("ConditionsFactory Test") {
    ConditionsFactory conditionsFactory;

    SUBCASE("Simple Conditions") {
        std::vector<std::string> conditionsTypes = conditionsFactory.getSimpleConditionTypes();

        for (const auto& type : conditionsTypes) {

            std::string senderID = "some_id";
            std::string validationValue = "some_value";

            std::shared_ptr<ConditionBase> condition = conditionsFactory.createSimpleCondition(senderID, type, validationValue);

            CHECK(condition != nullptr);

            nlohmann::json expectedJson = {
                {"type", type},
                {"senderId", senderID},
                {"validationValue", validationValue}
            };

            CHECK(condition->toJson() == expectedJson);
        }
    }

    SUBCASE("Composite Conditions") {
        std::vector<std::string> compositeConditionsTypes = conditionsFactory.getCompositeConditionTypes();
        std::vector<std::string> simpleConditionsTypes = conditionsFactory.getSimpleConditionTypes();

        // for all kinds of composite-conditions, create all possible pairs of simple-conditions that it will contain
        for (const auto& compositeType : compositeConditionsTypes) {

            std::string senderID = "some_id";
            std::string validationValue = "some_value";

            for (const auto& simpleType_1 : simpleConditionsTypes) {

                auto simpleCondition_1 =
                    conditionsFactory.createSimpleCondition(senderID, simpleType_1, validationValue);

                for (const auto& simpleType_2 : simpleConditionsTypes) {
                    auto simpleCondition_2 =
                        conditionsFactory.createSimpleCondition(senderID, simpleType_2, validationValue);

                    CHECK(simpleCondition_1 != nullptr);
                    CHECK(simpleCondition_2 != nullptr);

                    auto compositeCondition =
                        conditionsFactory.createCompositeCondition(compositeType, simpleCondition_1, simpleCondition_2);

                    CHECK(compositeCondition != nullptr);

                    nlohmann::json expectedJson = {
                    {"type", compositeType},
                    {"lhs", simpleCondition_1->toJson()},
                    {"rhs", simpleCondition_2->toJson()},
                    {"elapsedTime", constants::MAX_ELAPSED_TIME}
                    };

                    CHECK(compositeCondition->toJson() == expectedJson);
                }
            }
        }
    }
}

TEST_CASE("ConditionsFactory Failure Test") {
    ConditionsFactory conditionsFactory;

    SUBCASE("Simple Conditions") {
        std::string invalidConditionType = "ABC invalid type XYZ";
        std::string senderID = "some_id";
        std::string validationValue = "some_value";

        try {
            conditionsFactory.createSimpleCondition(senderID, invalidConditionType, validationValue);
            FAIL("Expected exception was not thrown");
        }
        catch (const std::invalid_argument& e) {
            CHECK(std::string(e.what()) == "condition type: " + invalidConditionType + " is not an option");
        }
    }

    SUBCASE("Composite Conditions") {
        std::string invalidConditionType = "ABC invalid type XYZ";

        std::vector<std::string> simpleConditionsTypes = conditionsFactory.getSimpleConditionTypes();
        std::string senderID = "some_id";
        std::string validationValue = "some_value";

        std::shared_ptr<ConditionBase> simpleCondition_1 =
            conditionsFactory.createSimpleCondition(senderID, simpleConditionsTypes.at(0), validationValue);
        std::shared_ptr<ConditionBase> simpleCondition_2 =
            conditionsFactory.createSimpleCondition(senderID, simpleConditionsTypes.at(1), validationValue);;

        try {
            conditionsFactory.createCompositeCondition(invalidConditionType, simpleCondition_1, simpleCondition_2);
            FAIL("Expected exception was not thrown");
        }
        catch (const std::invalid_argument& e) {
            // Assert the exception message
            CHECK(std::string(e.what()) == "condition type: " + invalidConditionType + " is not an option");
        }
    }
}

TEST_CASE("Extended Failing AndCondition Test Cases") {
    const std::string senderId = "testSender";

    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<StartsWithCondition>(senderId, "hello");
        auto trueCond2 = std::make_shared<EndsWithCondition>(senderId, "world");
        AndCondition andCond(trueCond1, trueCond2, std::chrono::milliseconds(100));
        CHECK(andCond.validate(senderId, "hello world")); 
    }

    SUBCASE("Both Conditions False") {
        auto falseCond1 = std::make_shared<StartsWithCondition>(senderId, "world");
        auto falseCond2 = std::make_shared<EndsWithCondition>(senderId, "hello");
        AndCondition andCond(falseCond1, falseCond2, std::chrono::milliseconds(100));
        CHECK_FALSE(andCond.validate(senderId, "hello world"));
    }

    SUBCASE("One Condition True, One False") {
        auto trueCond = std::make_shared<StartsWithCondition>(senderId, "hello");
        auto falseCond = std::make_shared<EndsWithCondition>(senderId, "unknown");
        AndCondition andCond(trueCond, falseCond, std::chrono::milliseconds(100));
        CHECK_FALSE(andCond.validate(senderId, "hello world")); 
    }

    SUBCASE("Elapsed Time Exceeds") {
        auto trueCond = std::make_shared<StartsWithCondition>(senderId, "hello");
        auto falseCond = std::make_shared<EndsWithCondition>(senderId, "unknown");
        AndCondition andCond(trueCond, falseCond, std::chrono::milliseconds(10));
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        CHECK_FALSE(andCond.validate(senderId, "hello world")); 
    }

    SUBCASE("Conditions with Empty Strings") {
        auto emptyCond1 = std::make_shared<StartsWithCondition>(senderId, "");
        auto emptyCond2 = std::make_shared<EndsWithCondition>(senderId, "");
        AndCondition andCond(emptyCond1, emptyCond2, std::chrono::milliseconds(100));

        CHECK(andCond.validate(senderId, ""));

        CHECK_FALSE(andCond.validate(senderId, "non-empty"));
    }
}
/*
* In order to make this test reliable, we need to change
* constants::CONDITIONS_JSON_FILE_NAME to something more
* generic like C:/path/to/file and not just a file name
* AND WHILE DOING SO, we need to consider windows/linux differences
* 
TEST_CASE("Loading Json files") {
    std::string tempFileName = "ConditionLogicTemp.json";
    int success = std::rename(constants::CONDITIONS_JSON_FILE_NAME.c_str(), tempFileName.c_str());
    if (success != 0)
        FAIL("std::rename() failed");

    nlohmann::json::array_t jsonData;
    for (int i = 0; i < 100; i++)
    {
        jsonData.push_back({
            {"scenario", std::to_string(i)},
            {"data", "some data"}
            });
    }

    SUBCASE("Save and Load a file") {
        JsonLoader().saveConditionsLogic(jsonData);
        nlohmann::json::array_t fileData = JsonLoader().loadConditionsLogic();
        CHECK(fileData == jsonData);
    }

    std::remove(constants::CONDITIONS_JSON_FILE_NAME.c_str());
    std::rename(tempFileName.c_str(), constants::CONDITIONS_JSON_FILE_NAME.c_str());
}
*/