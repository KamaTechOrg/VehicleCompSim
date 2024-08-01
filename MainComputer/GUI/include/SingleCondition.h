#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

#include "ConditionLayoutBase.h"

class SingleCondition : public ConditionLayoutBase
{
	Q_OBJECT

public:
	SingleCondition();
	~SingleCondition();

	void setAndOrButton(bool And = true);
	void deleteAndOrButton();
	void andOrButtonSwitch();
	std::shared_ptr<ConditionBase> data() override;
	conditionType getConditionType();

signals:
	void requestDelete(ConditionLayoutBase* layout);

private:
	QHBoxLayout*	_SingleCondition;
	QComboBox*		_inputSource;
	QComboBox*		_conditionType;
	QLineEdit*		_validationValue;
	QPushButton*	_andOrButton;
	QPushButton*	_deleteButton;
};