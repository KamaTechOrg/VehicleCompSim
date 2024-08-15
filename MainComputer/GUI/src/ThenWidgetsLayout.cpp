#include "ThenWidgetsLayout.h"

ThenWidgetsLayout::ThenWidgetsLayout(QWidget* parent)
	: QHBoxLayout(parent)
{
	_targetUnit = new QComboBox(parent);
	_targetUnit->setPlaceholderText("target unit");
	_targetUnit->addItem("target 1");
	_targetUnit->addItem("target 2");
	addWidget(_targetUnit);

	_operation = new QComboBox(parent);
	_operation->setPlaceholderText("operation");
	_operation->addItem("do 1");
	_operation->addItem("do 2");
	_operation->addItem("do 3");
	_operation->addItem("do 4");
	addWidget(_operation);

	addStretch(1);
}
