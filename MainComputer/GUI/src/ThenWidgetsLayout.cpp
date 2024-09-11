#include "ThenWidgetsLayout.h"

#include <QDebug>

#include "ControllersManager.h"

ThenWidgetsLayout::ThenWidgetsLayout(QWidget* parent)
	: QHBoxLayout(parent)
{
	_messagePart1 = new QLabel;
	_messagePart1->setText("Send a message to:");
	addWidget(_messagePart1);

	_targetUnit = new QComboBox(parent);
	_targetUnit->setPlaceholderText("target unit");
	std::vector<std::string> controllers = ControllersManager().getControllersIDS();
	for (const auto& constroller : controllers)
		_targetUnit->addItem(constroller.c_str());
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

std::shared_ptr<Action> ThenWidgetsLayout::data()
{
	std::string targetUnit;
	std::string message;

	try {
		targetUnit = _targetUnit->currentText().toStdString();
	}
	catch (const std::exception& e) {
		qWarning() << "Error: " << e.what();
	}

	message = _operation->currentText().toStdString();

	std::shared_ptr<Action> action = std::make_shared<Action>(targetUnit, message);
	return action;
}

unsigned ThenWidgetsLayout::extractIdFromString(const std::string& str)
{
	// Find the last space in the string
	size_t pos = str.find_last_of(' ');
	if (pos == std::string::npos) {
		throw std::invalid_argument("String format is incorrect.");
	}

	// Extract the substring after the last space
	std::string idStr = str.substr(pos + 1);

	// Convert the extracted substring to an unsigned integer
	unsigned id = std::stoul(idStr);

	return id;
}
