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

	std::shared_ptr<ConditionBase> data() override;

signals:
	void requestDelete(ConditionLayoutBase* layout);

private:
	QHBoxLayout*	_layout;
	QComboBox*		_inputSource;
	QComboBox*		_conditionType;
	QLineEdit*		_validationValue;
	QPushButton*	_deleteButton;
};