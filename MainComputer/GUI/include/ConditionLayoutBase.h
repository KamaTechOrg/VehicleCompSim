#pragma once

#include <QVBoxLayout>
#include "ConditionBase.h"

/**
 * @brief Base class for graphical layouts of user-defined conditions.
 *
 * This layout serves as a base for organizing condition elements in a vertical arrangement.
 */

class ConditionLayoutBase : public QVBoxLayout
{
	Q_OBJECT

public:
	virtual ~ConditionLayoutBase() = default;

	enum conditionType
	{
		And, Or, Null
	};

	virtual std::shared_ptr<ConditionBase> logicData()  = 0;
	virtual nlohmann::json GuiData() = 0;
};