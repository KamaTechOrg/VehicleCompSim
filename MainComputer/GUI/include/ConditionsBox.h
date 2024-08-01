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

	std::shared_ptr<ConditionBase> data();

private:
	QVBoxLayout* _layout;
	std::vector<ConditionLayoutBase*> _conditionsGroup;
	QPushButton* _addConditionGroup;

	void addConditionGroup(bool operationButton = false);
	void deleteGroup(ConditionLayoutBase* group);
	void createAddGroupButton();
	void addButtonClicked();
	std::shared_ptr<ConditionBase> buildTree(const std::vector<std::shared_ptr<ConditionBase>>& conditions, const std::vector<ConditionLayoutBase::conditionType>& operators);
};