#pragma once

#include <QGroupBox>
#include <QPushButton.h>


#include "SingleCondition.h"
#include "ConditionsGroup.h"

class ConditionsBox : public QGroupBox
{
	Q_OBJECT

public:
	ConditionsBox();
	~ConditionsBox();

	ConditionBase* data();

private:
	QVBoxLayout* _layout;
	std::vector<ConditionLayoutBase*> _conditionsGroup;
	QPushButton* _addConditionGroup;

	void addConditionGroup(bool operationButton = false);
	void deleteGroup(ConditionLayoutBase* group);
	void createAddGroupButton();
	void addButtonClicked();
	ConditionBase* buildTree(const std::vector<ConditionBase*>& conditions, const std::vector<ConditionLayoutBase::conditionType>& operators);
};