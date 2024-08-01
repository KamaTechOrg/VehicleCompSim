#include "BinaryTreeBuilder.h"
#include "SimpleCondition.h"
#include "AndCondition.h"
#include "OrCondition.h"
#include "ConditionsManager.h"
#include <stdexcept>
#include <stack>
#include <fstream>

ConditionBase* BinaryTreeBuilder::buildTree(const std::vector<ConditionBase*>& conditions, const std::vector<ConditionLayoutBase::conditionType>& operators) {
    /*
    if (conditions.empty()) {
        return nullptr;
    }
    if (conditions.size() != operators.size() + 1) {
        throw std::runtime_error("Number of conditions and operators don't match");
    }

    ConditionBase* root = nullptr;
    ConditionBase* current = root;

    for (size_t i = 0; i < operators.size(); ++i) {
        if (i == conditions.size() - 1) // it's the last one in the vector
        {
            current = conditions[i];
        }
        else
        {
            ConditionBase* next = nullptr;
            if (operators[i] == ConditionLayoutBase::conditionType::And)
                current = new AndCondition(conditions[i], next);
            else if (operators[i] == ConditionLayoutBase::conditionType::Or)
                current = new OrCondition(conditions[i], next);
            current = next;
        }
    }
    return root;
    */
    return nullptr;
}

//void BinaryTreeBuilder::exportToJson(const std::string& filename) {
//    if (root) {
//        exportBinaryTreeToJson(root, filename);
//    }
//}

void BinaryTreeBuilder::exportBinaryTreeToJson(ConditionBase* root, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    nlohmann::json j;
    j = root->toJson();

    file << j.dump(4); 
    file.close();
}
