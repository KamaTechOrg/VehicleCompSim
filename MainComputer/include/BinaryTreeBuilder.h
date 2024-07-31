#ifndef BINARY_TREE_BUILDER_H
#define BINARY_TREE_BUILDER_H

#include <vector>
#include <string>
#include "ConditionBase.h"
#include "ConditionLayoutBase.h"

class SimpleCondition;

class BinaryTreeBuilder {
private:
    void exportBinaryTreeToJson(ConditionBase* root, const std::string& filename);
public:
    BinaryTreeBuilder() = default;
    ~BinaryTreeBuilder() = default;
    ConditionBase* buildTree(const std::vector<ConditionBase*>& conditions, const std::vector<ConditionLayoutBase::conditionType>& operators);
    //void exportToJson(const std::string& filename);
};

#endif 
