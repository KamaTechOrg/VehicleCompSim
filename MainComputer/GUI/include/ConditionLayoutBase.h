#pragma once

#include <QVBoxLayout>
#include "ConditionBase.h"

class ConditionLayoutBase : public QVBoxLayout
{
	Q_OBJECT

public:
	virtual ~ConditionLayoutBase() = default;

	enum conditionType
	{
		And, Or, Null
	};

	virtual void setAndOrButton(bool And = true)   = 0;
	virtual void deleteAndOrButton()			   = 0;
	virtual void andOrButtonSwitch()			   = 0;
	virtual std::shared_ptr<ConditionBase> data()  = 0;
	virtual conditionType getConditionType()	   = 0;
};