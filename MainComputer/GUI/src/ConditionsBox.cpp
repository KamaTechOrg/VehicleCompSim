#include "ConditionsBox.h"

#include <iostream>
#include <fstream>
#include "BinaryTreeBuilder.h"
#include "SimpleCondition.h"
#include "AndCondition.h"
#include "OrCondition.h"

ConditionsBox::ConditionsBox()
	: QGroupBox("if")
{
	_layout = new QVBoxLayout;
	createAddGroupButton();
	setLayout(_layout);
}

ConditionsBox::~ConditionsBox()
{
	if (_layout)
		delete _layout;
	if (_addConditionGroup)
		delete _addConditionGroup;
}

ConditionBase* ConditionsBox::data()
{
	std::vector<ConditionBase*> conditions;
	std::vector<ConditionLayoutBase::conditionType> operatorsType;

	for (auto it : _conditionsGroup)
	{
		ConditionBase* condition = it->data();
		if (condition != nullptr)
			conditions.push_back(condition);

		ConditionLayoutBase::conditionType type = it->getConditionType();
		if (type != ConditionLayoutBase::conditionType::Null)
			operatorsType.push_back(type);
	}
	return buildTree(conditions, operatorsType);
}

void ConditionsBox::addConditionGroup(bool operationButton)
{
	ConditionsGroup* group = new ConditionsGroup;
	if (operationButton == true && _conditionsGroup.empty() == false)
	{
		group->setAndOrButton();
	}
	_layout->addLayout(group);
	_conditionsGroup.push_back(group);

	connect(group, &ConditionsGroup::requestDelete, this, &ConditionsBox::deleteGroup);
}

void ConditionsBox::deleteGroup(ConditionLayoutBase* group)
{

	if (_conditionsGroup.size() > 1 && _conditionsGroup.at(0) == group)
	{
		_conditionsGroup.at(1)->deleteAndOrButton();
		// it becames the first one, so no need in this button anymore
	}
	_layout->removeItem(group);
	_conditionsGroup.erase(std::remove(_conditionsGroup.begin(), _conditionsGroup.end(), group), _conditionsGroup.end());
	group->deleteLater();
	_layout->update();
}

void ConditionsBox::createAddGroupButton()
{
	_addConditionGroup = new QPushButton("+");
	int defaultHeight = _addConditionGroup->sizeHint().height();
	_addConditionGroup->setFixedSize(defaultHeight, defaultHeight);
	connect(_addConditionGroup, &QPushButton::clicked, this, &ConditionsBox::addButtonClicked);
	_layout->addWidget(_addConditionGroup);
}

void ConditionsBox::addButtonClicked()
{
	if (_addConditionGroup)
		delete _addConditionGroup;

	addConditionGroup(true);
	createAddGroupButton();
}

ConditionBase* ConditionsBox::buildTree(const std::vector<ConditionBase*>& conditions, const std::vector<ConditionLayoutBase::conditionType>& operators)
{
	/*
	* TODO: assert that there are no nullptr's in the vector
	*/

	if (conditions.empty()) {
		return nullptr;
	}

	ConditionBase* root = conditions[0];
	ConditionBase* current = root;

	for (size_t i = 1; i < conditions.size(); ++i) {
		ConditionBase* next = conditions[i];
		if (operators[i - 1] == ConditionLayoutBase::conditionType::And) {
			current = new AndCondition(current, next);
		}
		else if (operators[i - 1] == ConditionLayoutBase::conditionType::Or) {
			current = new OrCondition(current, next);
		}
		root = current; // Update root to the current node for the next iteration
	}

	return root;
}