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

void ConditionsGroup::addSingleCondition(const int currentSourceIndex, const int currentTypeIndex, const std::string& currentValidationValue,
	const std::string& conditionType, const int elapsedTime)
{
	SingleCondition* conditionLayout = new SingleCondition(currentSourceIndex, currentTypeIndex, currentValidationValue);
	connect(conditionLayout, &SingleCondition::requestDelete, this, &ConditionsGroup::deleteCondition);
	addGenericCondition(conditionLayout, conditionType, elapsedTime);
}

void ConditionsGroup::addConditionsGroup(nlohmann::json jsonData)
{
	for (size_t i = 0; i < jsonData.size(); i += 2)
	{
		const auto& item = jsonData[i];
		ConditionLayoutBase* newCondition;

		// If the item is a single condition
		if (item.is_object())
		{
			int sourceIndex = item["inputSource"];
			int typeIndex = item["conditionType"];
			std::string validationValue = item["validationValue"];

			newCondition = new SingleCondition(sourceIndex, typeIndex, validationValue);
			connect(dynamic_cast<SingleCondition*>(newCondition), &SingleCondition::requestDelete, this, &ConditionsGroup::deleteCondition);

		}
		// If the item is a list (group of conditions)
		else if (item.is_array())
		{
			newCondition = new ConditionsGroup();
			dynamic_cast<ConditionsGroup*>(newCondition)->addConditionsGroup(item);
			connect(dynamic_cast<ConditionsGroup*>(newCondition), &ConditionsGroup::requestDelete, this, &ConditionsGroup::deleteCondition);
		}
		else
		{
			throw std::runtime_error("invalid JSON GUI file");
		}

		// Add the operation between two conditons (And/Or, elapsedTime)
		if (i == 0)
		{
			addGenericCondition(newCondition);
		}
		else
		{
			addGenericCondition(newCondition, jsonData[i - 1]["type"], jsonData[i - 1]["elapsedTime"]);
		}
	}
}


void ConditionsGroup::addEmptySingleCondition()
{
	SingleCondition* conditionLayout = new SingleCondition;
	connect(conditionLayout, &SingleCondition::requestDelete, this, &ConditionsGroup::deleteCondition);
	addGenericCondition(conditionLayout);
}

void ConditionsGroup::addEmptyConditionsGroup()
{
	ConditionsGroup* conditionLayout = new ConditionsGroup;
	connect(conditionLayout, &ConditionsGroup::requestDelete, this, &ConditionsGroup::deleteCondition);
	addGenericCondition(conditionLayout);
}

void ConditionsGroup::addGenericCondition(ConditionLayoutBase* condition,
	const std::string& andOrValue, const int elapsedTimeValue)
{
	_conditions.push_back(condition);
	if (_conditions.size() > 1) // it's not the first condition added
	{
		QPushButton* andOrButton = createAndOrButton(andOrValue);
		QSpinBox* elapsedTime = createElapsedTimeWidget(elapsedTimeValue);

		QHBoxLayout* operationLayout = new QHBoxLayout;
		operationLayout->addWidget(andOrButton);
		operationLayout->addWidget(elapsedTime);
		operationLayout->addStretch(1);
		_conditionsLayout->addLayout(operationLayout);

		_operations.push_back(operationLayout);
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
	addEmptySingleCondition();
}

void ConditionsGroup::addGroupButtonClicked()
{
	addEmptyConditionsGroup();
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
		if (andOrButton->text() == "And") {
			current = std::make_shared<AndCondition>(AndCondition(current, next, std::chrono::milliseconds(elapsedTime->value())));
		}
		else if (andOrButton->text() == "Or") {
			current = std::make_shared<OrCondition>(OrCondition(current, next, std::chrono::milliseconds(elapsedTime->value())));
		}
		root = current; // Update root to the current node for the next iteration
	}

	return root;
}

std::shared_ptr<ConditionBase> ConditionsGroup::logicData()
{
	std::vector<std::shared_ptr<ConditionBase>> conditions;
	for (auto it : _conditions)
	{
		std::shared_ptr<ConditionBase> condition = it->logicData();
		if (condition == nullptr) return nullptr;
		conditions.push_back(condition);
	}
	return buildTree(conditions);
}

QPushButton* ConditionsGroup::createAndOrButton(const std::string& buttonValue)
{
	QPushButton* andOrButton = new QPushButton(buttonValue.c_str());
	connect(andOrButton, &QPushButton::clicked, [andOrButton]() {
		andOrButton->setText(andOrButton->text() == "And" ? "Or" : "And");
		});
	int defaultHeight = andOrButton->sizeHint().height();
	andOrButton->setFixedSize(defaultHeight * 3, defaultHeight);

	return andOrButton;
}

QSpinBox* ConditionsGroup::createElapsedTimeWidget(const int value)
{
	QSpinBox* elapsedTime = new QSpinBox;
	elapsedTime->setRange(0, constants::MAX_ELAPSED_TIME);
	elapsedTime->setValue(value);
	elapsedTime->setSuffix(" ms");
	int defaultHeight = elapsedTime->sizeHint().height();
	elapsedTime->setFixedSize(defaultHeight * 5, defaultHeight);

	return elapsedTime;
}

nlohmann::json ConditionsGroup::GuiData()
{
	nlohmann::json conditionsArray = nlohmann::json::array();

	for (int i = 0; i < _conditions.size(); i++) {
		conditionsArray.push_back(_conditions[i]->GuiData());

		if (i == _operations.size()) {
			continue;
		}
		
		QPushButton* andOrButton = qobject_cast<QPushButton*>(_operations[i]->itemAt(0)->widget());
		QSpinBox* elapsedTime = qobject_cast<QSpinBox*>(_operations[i]->itemAt(1)->widget());

		nlohmann::json operation = {
			{"type", andOrButton->text().toStdString()},
			{"elapsedTime", elapsedTime->value()}
		};

		conditionsArray.push_back(operation);
	}

	return conditionsArray;
}