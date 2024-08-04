#include "doctest.h"
#include "../include/AndCondition.h"
#include "../include/OrCondition.h"
#include "../include/SimpleCondition.h"
#include <memory>

// Mock conditions
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

// Tests for SimpleCondition
TEST_CASE("SimpleCondition Tests") {
    SUBCASE("Equals To Validation") {
        SimpleCondition cond("value", "equals to", "value");
        CHECK(cond.validate());
        CHECK(cond.toJson() == nlohmann::json({
            {"input", "value"},
            {"validationType", "equals to"},
            {"validationValue", "value"}
            }));
    }

    SUBCASE("Greater Than Validation") {
        SimpleCondition cond("10", "greater than", "5");
        CHECK(cond.validate());
        CHECK(cond.toJson() == nlohmann::json({
            {"input", "10"},
            {"validationType", "greater than"},
            {"validationValue", "5"}
            }));
    }

    SUBCASE("Smaller Than Validation") {
        SimpleCondition cond("3", "smaller than", "5");
        CHECK(cond.validate());
        CHECK(cond.toJson() == nlohmann::json({
            {"input", "3"},
            {"validationType", "smaller than"},
            {"validationValue", "5"}
            }));
    }

    SUBCASE("Starts With Validation") {
        SimpleCondition cond("hello world", "starts with", "hello");
        CHECK(cond.validate());
        CHECK(cond.toJson() == nlohmann::json({
            {"input", "hello world"},
            {"validationType", "starts with"},
            {"validationValue", "hello"}
            }));
    }

    SUBCASE("Ends With Validation") {
        SimpleCondition cond("hello world", "ends with", "world");
        CHECK(cond.validate());
        CHECK(cond.toJson() == nlohmann::json({
            {"input", "hello world"},
            {"validationType", "ends with"},
            {"validationValue", "world"}
            }));
    }
}

// Tests for AndCondition
TEST_CASE("AndCondition Tests") {
    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<SimpleCondition>("value", "starts with", "val");
        auto trueCond2 = std::make_shared<SimpleCondition>("value", "ends with", "ue");
        AndCondition andCond(trueCond1, trueCond2);
        CHECK(andCond.validate());
        CHECK(andCond.toJson() == nlohmann::json({
            {"type", "AndCondition"},
            {"elapsedTime", std::to_string(MAX_ELAPSED_TIME)},
            {"LHS", nlohmann::json({
                {"input", "value"},
                {"validationType", "starts with"},
                {"validationValue", "val"}
            })},
            {"RHS", nlohmann::json({
                {"input", "value"},
                {"validationType", "ends with"},
                {"validationValue", "ue"}
            })}
            }));
    }

    SUBCASE("One Condition False") {
        auto trueCond = std::make_shared<SimpleCondition>("value", "starts with", "val");
        auto falseCond = std::make_shared<SimpleCondition>("value", "ends with", "world");
        AndCondition andCond(trueCond, falseCond);
        CHECK_FALSE(andCond.validate());
        CHECK(andCond.toJson() == nlohmann::json({
            {"type", "AndCondition"},
            {"elapsedTime", std::to_string(MAX_ELAPSED_TIME)},
            {"LHS", nlohmann::json({
                {"input", "value"},
                {"validationType", "starts with"},
                {"validationValue", "val"}
            })},
            {"RHS", nlohmann::json({
                {"input", "value"},
                {"validationType", "ends with"},
                {"validationValue", "world"}
            })}
            }));
    }
}

// Tests for OrCondition
TEST_CASE("OrCondition Tests") {
    SUBCASE("Both Conditions True") {
        auto trueCond1 = std::make_shared<SimpleCondition>("hello world", "starts with", "hello");
        auto trueCond2 = std::make_shared<SimpleCondition>("hello world", "ends with", "world");
        OrCondition orCond(trueCond1, trueCond2);
        CHECK(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "OrCondition"},
            {"elapsedTime", std::to_string(MAX_ELAPSED_TIME)},
            {"LHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "starts with"},
                {"validationValue", "hello"}
            })},
            {"RHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "ends with"},
                {"validationValue", "world"}
            })}
            }));
    }

    SUBCASE("One Condition True") {
        auto trueCond = std::make_shared<SimpleCondition>("hello world", "starts with", "hello");
        auto falseCond = std::make_shared<SimpleCondition>("hello world", "ends with", "unknown");
        OrCondition orCond(trueCond, falseCond);
        CHECK(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "OrCondition"},
            {"elapsedTime", std::to_string(MAX_ELAPSED_TIME)},
            {"LHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "starts with"},
                {"validationValue", "hello"}
            })},
            {"RHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "ends with"},
                {"validationValue", "unknown"}
            })}
            }));
    }

    SUBCASE("Both Conditions False") {
        auto falseCond1 = std::make_shared<SimpleCondition>("hello world", "starts with", "world");
        auto falseCond2 = std::make_shared<SimpleCondition>("hello world", "ends with", "hello");
        OrCondition orCond(falseCond1, falseCond2);
        CHECK_FALSE(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "OrCondition"},
            {"elapsedTime", std::to_string(MAX_ELAPSED_TIME)},
            {"LHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "starts with"},
                {"validationValue", "world"}
            })},
            {"RHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "ends with"},
                {"validationValue", "hello"}
            })}
            }));
    }
}


TEST_CASE("elapsedTime test") {
    SUBCASE("orCondition with elapsedTime") {
        auto trueCond1 = std::make_shared<SimpleCondition>("hello world", "starts with", "hello");
        auto trueCond2 = std::make_shared<SimpleCondition>("hello world", "ends with", "world");
        OrCondition orCond(trueCond1, trueCond2, std::chrono::milliseconds(1024));
        CHECK(orCond.validate());
        CHECK(orCond.toJson() == nlohmann::json({
            {"type", "OrCondition"},
            {"elapsedTime", std::to_string(1024)},
            {"LHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "starts with"},
                {"validationValue", "hello"}
            })},
            {"RHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "ends with"},
                {"validationValue", "world"}
            })}
            }));
    }

    SUBCASE("andCondition with elapsedTime") {
        auto trueCond1 = std::make_shared<SimpleCondition>("hello world", "starts with", "hello");
        auto trueCond2 = std::make_shared<SimpleCondition>("hello world", "ends with", "world");
        AndCondition andCond(trueCond1, trueCond2, std::chrono::milliseconds(1024));
        CHECK(andCond.validate());
        CHECK(andCond.toJson() == nlohmann::json({
            {"type", "AndCondition"},
            {"elapsedTime", std::to_string(1024)},
            {"LHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "starts with"},
                {"validationValue", "hello"}
            })},
            {"RHS", nlohmann::json({
                {"input", "hello world"},
                {"validationType", "ends with"},
                {"validationValue", "world"}
            })}
            }));
    }
}