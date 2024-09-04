#include "ThenWidgetsLayout.h"

ThenWidgetsLayout::ThenWidgetsLayout(QWidget* parent)
	: QHBoxLayout(parent)
{
	_messagePart1 = new QLabel;
	_messagePart1->setText("Send a message to:");
	addWidget(_messagePart1);

	_targetUnit = new QComboBox(parent);
	_targetUnit->setPlaceholderText("target unit");
	_targetUnit->addItem("target 1");
	_targetUnit->addItem("target 2");
	addWidget(_targetUnit);

	_messagePart2 = new QLabel;
	_messagePart2->setText("to do:");
	addWidget(_messagePart2);

	_operation = new QComboBox(parent);
	_operation->setPlaceholderText("operation");
	_operation->addItem("do 1");
	_operation->addItem("do 2");
	_operation->addItem("do 3");
	_operation->addItem("do 4");
	addWidget(_operation);

	addStretch(1);
}
