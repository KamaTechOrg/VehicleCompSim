#pragma once

#include <QHBoxLayout>
#include <QComboBox>

class ThenWidgetsLayout : public QHBoxLayout
{
public:
	ThenWidgetsLayout(QWidget* parent = nullptr);

private:
	QComboBox* _targetUnit;
	QComboBox* _operation;
};