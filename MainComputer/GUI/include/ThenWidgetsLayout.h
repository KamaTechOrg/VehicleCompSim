#pragma once

#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

#include "Action.h"

class ThenWidgetsLayout : public QHBoxLayout
{
public:
	ThenWidgetsLayout(QWidget* parent = nullptr);

	std::shared_ptr<Action> data();

private:
	QLabel* _messagePart1;
	QLabel* _messagePart2;
	QComboBox* _targetUnit;
	QComboBox* _operation;

	unsigned extractIdFromString(const std::string& str);
};