#include "ConditionsGroup.h"

#include <fstream>
#include <QMenu>

#include "SimpleCondition.h"
#include <BinaryTreeBuilder.h>
#include <CompositeCondition.h>
#include <AndCondition.h>
#include <OrCondition.h>

ConditionsGroup::ConditionsGroup()
	: _andOrButton(nullptr)
{
	_conditionsBox = new QGroupBox;
	_conditionsLayout = new QVBoxLayout;
	_buttonsLayout = new QHBoxLayout;
	
	addLayout(_buttonsLayout);
	_conditionsBox->setLayout(_conditionsLayout);
	addWidget(_conditionsBox);

	createAddConditionButton();
	createDeleteButton();
}

ConditionsGroup::~ConditionsGroup()
{
	if (_conditionsBox)
		delete _conditionsBox;
	
	if (_andOrButton)
		delete _andOrButton;

	if (_deleteButton)
		delete _deleteButton;

	_conditions.clear();
}

void ConditionsGroup::addSingleCondition(bool operationButton)
{
	SingleCondition* widgetsLayout = new SingleCondition;
	if (operationButton == true && _conditions.empty() == false)
	{
		widgetsLayout->setAndOrButton();
	}
	_conditionsLayout->addLayout(widgetsLayout);
	_conditions.push_back(widgetsLayout);

	connect(widgetsLayout, &SingleCondition::requestDelete, this, &ConditionsGroup::deleteCondition);
}

void ConditionsGroup::addConditionGroup(bool operationButton)
{
	ConditionsGroup* widgetsLayout = new ConditionsGroup;
	if (operationButton == true && _conditions.empty() == false)
	{
		widgetsLayout->setAndOrButton();
	}
	_conditionsLayout->addLayout(widgetsLayout);
	_conditions.push_back(widgetsLayout);

	connect(widgetsLayout, &ConditionsGroup::requestDelete, this, &ConditionsGroup::deleteCondition);
}

void ConditionsGroup::createAddConditionButton()
{
	_addConditionButton = new QPushButton("+");
	int defaultHeight = _addConditionButton->sizeHint().height();
	_addConditionButton->setFixedSize(defaultHeight, defaultHeight);
	//connect(_addConditionButton, &QPushButton::clicked, this, &ConditionsGroup::addButtonClicked);

	QMenu* menu = new QMenu();
	QAction* addSingleConditionAction = new QAction("Add Single Condition");
	QAction* addConditionGroupAction = new QAction("Add Condition Group");

	menu->addAction(addSingleConditionAction);
	menu->addAction(addConditionGroupAction);

	connect(addSingleConditionAction, &QAction::triggered, this, &ConditionsGroup::addSingleButtonClicked);
	connect(addConditionGroupAction, &QAction::triggered, this, &ConditionsGroup::addGroupButtonClicked);

	// connect the button to show the menu
	connect(_addConditionButton, &QPushButton::clicked, [this, menu]() {
		menu->exec(QCursor::pos());
	});

	_conditionsLayout->addWidget(_addConditionButton);
	//_buttonsLayout->addStretch(1);
}

void ConditionsGroup::createDeleteButton()
{
	_deleteButton = new QPushButton("-");
	int defaultHeight = _deleteButton->sizeHint().height();
	_deleteButton->setFixedSize(defaultHeight, defaultHeight);
	_buttonsLayout->addWidget(_deleteButton);
	_buttonsLayout->addStretch(1);

	connect(_deleteButton, &QPushButton::clicked, this, [this]() {
		emit requestDelete(this);
	});
}

void ConditionsGroup::deleteCondition(ConditionLayoutBase* layout)
{
	if (_conditions.size() > 1 && _conditions.at(0) == layout)
	{
		_conditions.at(1)->deleteAndOrButton();
		// it becames the first one, so no need in this button anymore
	}
	_conditionsLayout->removeItem(layout);
	_conditions.erase(std::remove(_conditions.begin(), _conditions.end(), layout), _conditions.end());
	layout->deleteLater();
	_conditionsLayout->update();
}

void ConditionsGroup::addSingleButtonClicked()
{
	if (_addConditionButton)
		delete _addConditionButton;

	addSingleCondition(true);
	createAddConditionButton();
}

void ConditionsGroup::addGroupButtonClicked()
{
	if (_addConditionButton)
		delete _addConditionButton;

	addConditionGroup(true);
	createAddConditionButton();
}

void ConditionsGroup::setAndOrButton(bool And)
{
	if (!_andOrButton)
	{
		_andOrButton = new QPushButton(And == true ? "and" : "or");
		int defaultHeight = _andOrButton->sizeHint().height();
		_andOrButton->setFixedSize(defaultHeight * 3, defaultHeight);
		_buttonsLayout->insertWidget(0, _andOrButton);

		connect(_andOrButton, &QPushButton::clicked, this, &ConditionsGroup::andOrButtonSwitch);
	}
	else
	{
		_andOrButton->setText(And == true ? "and" : "or");
	}
}

void ConditionsGroup::deleteAndOrButton()
{
	_andOrButton->deleteLater();
	delete _andOrButton;
	_andOrButton = nullptr;
}

void ConditionsGroup::andOrButtonSwitch()
{
	QString text = _andOrButton->text();
	if (text == "or")
		_andOrButton->setText("and");
	else
		_andOrButton->setText("or");
}

ConditionBase* ConditionsGroup::data()
{
	std::vector<ConditionBase*> conditions;
	std::vector<conditionType> operatorsType;

	for (auto it : _conditions)
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

ConditionLayoutBase::conditionType ConditionsGroup::getConditionType()
{
	if (_andOrButton != nullptr)
	{
		if (_andOrButton->text().toStdString() == "and")
			return conditionType::And;
		else if (_andOrButton->text().toStdString() == "or")
			return conditionType::Or;
	}
	return conditionType::Null;
}

ConditionBase* ConditionsGroup::buildTree(const std::vector<ConditionBase*>& conditions, const std::vector<ConditionLayoutBase::conditionType>& operators)
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