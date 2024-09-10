#pragma once

#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpinBox>

#include "json.hpp"
#include "constants.h"
#include "SingleCondition.h"
#include "ConditionLayoutBase.h"

class ConditionsGroup : public ConditionLayoutBase
{
	Q_OBJECT

public:
	ConditionsGroup();
	~ConditionsGroup();

	void unableDelete();
	void setBoxTitle(const char* title);
	void addSingleCondition(const int currentSourceIndex, const int currentTypeIndex, const std::string& currentValidationValue);
	void addConditionsGroup(nlohmann::json jsonData);

	std::shared_ptr<ConditionBase> logicData() override;
	nlohmann::json GuiData() override;

private:
	std::vector<ConditionLayoutBase*> _conditions;

	// contains the "and" / "or" operations between every two conditions in _conditions.
	// and also the elapsed time allowed between every two conditions
	std::vector<QHBoxLayout*> _operations;
	
	QHBoxLayout* _layout;
	QVBoxLayout* _boxLayout;
	QGroupBox* _conditionsBox;
	QVBoxLayout* _conditionsLayout;
	QPushButton* _addConditionButton;
	QPushButton* _deleteButton; // delete *this group button
	
	void addEmptySingleCondition();
	void addEmptyConditionsGroup();
	void addGenericCondition(ConditionLayoutBase* condition, const std::string &andOrValue = "And", const int elapsedTime = 0);
	void createAddConditionButton();
	void createDeleteButton();
	void deleteCondition(ConditionLayoutBase* layout);
	void addSingleButtonClicked();
	void addGroupButtonClicked();
	QPushButton* createAndOrButton(const std::string &buttonValue);
	QSpinBox* createElapsedTimeWidget(const int value);
	std::shared_ptr<ConditionBase> buildTree(const std::vector<std::shared_ptr<ConditionBase>> &conditions);
	
signals:
	void requestDelete(ConditionLayoutBase* group);
};