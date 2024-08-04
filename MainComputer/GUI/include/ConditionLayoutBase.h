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

	virtual std::shared_ptr<ConditionBase> data()  = 0;
};