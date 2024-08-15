#include "ConditionsGroup.h"

#include <fstream>
#include <QMenu>
#include <QSpinBox>

#include "SimpleCondition.h"
#include "BinaryTreeBuilder.h"
#include "CompositeCondition.h"
#include "AndCondition.h"
#include "OrCondition.h"

ConditionsGroup::ConditionsGroup()
{
	_layout = new QHBoxLayout;
	_boxLayout = new QVBoxLayout;
	_conditionsBox = new QGroupBox;
	_conditionsLayout = new QVBoxLayout;
	
	_boxLayout->addLayout(_conditionsLayout);
	_conditionsBox->setLayout(_boxLayout);
	_layout->addWidget(_conditionsBox);

	createDeleteButton();
	createAddConditionButton();

	addLayout(_layout);
}

ConditionsGroup::~ConditionsGroup()
{
	if (_conditionsBox != nullptr)
		delete _conditionsBox;

	if (_deleteButton != nullptr)
		delete _deleteButton;

	_conditions.clear();
}

void ConditionsGroup::unableDelete()
{
	_deleteButton->deleteLater(); // remove from GUI
	_deleteButton = nullptr;
}

void ConditionsGroup::setBoxTitle(const char* title)
{
	_conditionsBox->setTitle(title);
}

void ConditionsGroup::addSingleCondition()
{
	SingleCondition* conditionLayout = new SingleCondition;
	connect(conditionLayout, &SingleCondition::requestDelete, this, &ConditionsGroup::deleteCondition);
	addGenericCondition(conditionLayout);
}

void ConditionsGroup::addConditionsGroup()
{
	ConditionsGroup* conditionLayout = new ConditionsGroup;
	connect(conditionLayout, &ConditionsGroup::requestDelete, this, &ConditionsGroup::deleteCondition);
	addGenericCondition(conditionLayout);
}

void ConditionsGroup::addGenericCondition(ConditionLayoutBase* condition)
{
	_conditions.push_back(condition);
	if (_conditions.size() > 1) // it's not the first condition added
	{
		// create and/or button
		QPushButton* andOrButton = new QPushButton("and");
		connect(andOrButton, &QPushButton::clicked, [andOrButton]() {
			andOrButton->setText(andOrButton->text() == "and" ? "or" : "and");
			});
		int defaultHeight = andOrButton->sizeHint().height();
		andOrButton->setFixedSize(defaultHeight * 3, defaultHeight);

		// create elapsedTime edit line
		QSpinBox* elapsedTime = new QSpinBox;
		elapsedTime->setRange(0, MAX_ELAPSED_TIME);
		elapsedTime->setSuffix(" ms");
		defaultHeight = elapsedTime->sizeHint().height();
		elapsedTime->setFixedSize(defaultHeight * 5, defaultHeight);

		// push the both to a horizontal layout
		QHBoxLayout* operationLayout = new QHBoxLayout;
		operationLayout->addWidget(andOrButton);
		operationLayout->addWidget(elapsedTime);
		operationLayout->addStretch(1);

		// instert the layout to the _operations vector and to the _conditionsLayout
		_operations.push_back(operationLayout);
		_conditionsLayout->addLayout(operationLayout);
	}
	_conditionsLayout->addLayout(condition);
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

	_boxLayout->addWidget(_addConditionButton);
}

void ConditionsGroup::createDeleteButton()
{
	_deleteButton = new QPushButton("-");
	int defaultHeight = _deleteButton->sizeHint().height();
	_deleteButton->setFixedSize(defaultHeight, defaultHeight);
	_layout->addWidget(_deleteButton);

	connect(_deleteButton, &QPushButton::clicked, this, [this]() {
		emit requestDelete(this);
	});
}

void ConditionsGroup::deleteCondition(ConditionLayoutBase* layout)
{
	auto condition = std::find(_conditions.begin(), _conditions.end(), layout);
	int index = std::distance(_conditions.begin(), condition);
	
	// remove the condition itself:
	_conditions.erase(condition);
	_conditionsLayout->removeItem(layout);
	layout->deleteLater();

	int deleteOperationIndex = index > 0 ? index - 1 : index;

	if (!_operations.empty())
	{
		_conditionsLayout->removeItem(_operations.at(deleteOperationIndex));
		_operations.at(deleteOperationIndex)->itemAt(0)->widget()->deleteLater();
		_operations.at(deleteOperationIndex)->itemAt(1)->widget()->deleteLater();
		_operations.at(deleteOperationIndex)->deleteLater();
		_operations.erase(_operations.begin() + (deleteOperationIndex));
	}

	_conditionsLayout->update();
}

void ConditionsGroup::addSingleButtonClicked()
{
	addSingleCondition();
}

void ConditionsGroup::addGroupButtonClicked()
{
	addConditionsGroup();
}

std::shared_ptr<ConditionBase> ConditionsGroup::buildTree(const std::vector<std::shared_ptr<ConditionBase>>& conditions)
{
	/*
	* TODO: assert that there are no nullptr's in the vector
	*/

	if (conditions.empty()) {
		return nullptr;
	}

	std::shared_ptr<ConditionBase> root = conditions[0];
	std::shared_ptr<ConditionBase> current = root;

	for (size_t i = 1; i < conditions.size(); ++i) {
		if (conditions[i] == nullptr)
			continue;

		std::shared_ptr<ConditionBase> next = conditions[i];
		QPushButton* andOrButton = qobject_cast<QPushButton*>(_operations[i - 1]->itemAt(0)->widget());
		QSpinBox* elapsedTime = qobject_cast<QSpinBox*>(_operations[i - 1]->itemAt(1)->widget());
		if (andOrButton->text() == "and") {
			current = std::make_shared<AndCondition>(AndCondition(current, next, std::chrono::milliseconds(elapsedTime->value())));
		}
		else if (andOrButton->text() == "or") {
			current = std::make_shared<OrCondition>(OrCondition(current, next, std::chrono::milliseconds(elapsedTime->value())));
		}
		root = current; // Update root to the current node for the next iteration
	}

	return root;
}
std::shared_ptr<ConditionBase> ConditionsGroup::data()
{
	std::vector<std::shared_ptr<ConditionBase>> conditions;
	for (auto it : _conditions)
	{
		std::shared_ptr<ConditionBase> condition = it->data();
		if (condition == nullptr) return nullptr;
		conditions.push_back(condition);
	}
	return buildTree(conditions);
}