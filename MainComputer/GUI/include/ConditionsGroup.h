#pragma once

#include <QVBoxLayout>
#include <QGroupBox>

#include "SingleCondition.h"
#include "ConditionLayoutBase.h"

class ConditionsGroup : public ConditionLayoutBase
{
	Q_OBJECT

public:
	ConditionsGroup();
	~ConditionsGroup();

	std::shared_ptr<ConditionBase> data() override;

private:
	std::vector<ConditionLayoutBase*> _conditions;

	// contains the "and" / "or" operations between every two conditions in _conditions.
	// and also the elapsed time allowed between every two conditions
	std::vector<QHBoxLayout*> _operations;
	//std::vector<std::pair<std::unique_ptr<QPushButton>, std::unique_ptr<QLineEdit>>> _operations; // with safe pointers. we will later try using this instead.
	
	QVBoxLayout* _layout;
	QGroupBox* _conditionsBox;
	QVBoxLayout* _conditionsLayout;
	QPushButton* _addConditionButton;
	QPushButton* _deleteButton; // delete *this group button
	
	void addSingleCondition();
	void addConditionsGroup();
	void addGenericCondition(ConditionLayoutBase* condition);
	void createAddConditionButton();
	void createDeleteButton();
	void deleteCondition(ConditionLayoutBase* layout);
	void addSingleButtonClicked();
	void addGroupButtonClicked();
	std::shared_ptr<ConditionBase> buildTree(const std::vector<std::shared_ptr<ConditionBase>>& conditions);
	
signals:
	void requestDelete(ConditionLayoutBase* group);
};