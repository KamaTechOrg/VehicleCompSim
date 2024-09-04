#pragma once

#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

class ThenWidgetsLayout : public QHBoxLayout
{
public:
	ThenWidgetsLayout(QWidget* parent = nullptr);

private:
	QLabel* _messagePart1;
	QLabel* _messagePart2;
	QComboBox* _targetUnit;
	QComboBox* _operation;
};