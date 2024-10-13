#include <QDebug>
#include <QMessageBox>
#include <QHBoxLayout> 
#include "ThenWidgetsLayout.h"
#include "ControllersManager.h"

ThenWidgetsLayout::ThenWidgetsLayout(QWidget* parent)
	: QWidget(parent),
	_layout(new QHBoxLayout(this))
{
	setLayout(_layout);

	_messagePart1 = new QLabel;
	_messagePart1->setText("Send a message to:");
	addWidget(_messagePart1);

	_targetUnit = new QComboBox(this);
	_targetUnit->setPlaceholderText("target unit");
	std::vector<std::string> controllers = ControllersManager().getControllersIDS();
	for (const auto& controller : controllers)
		_targetUnit->addItem(controller.c_str());
	connect(_targetUnit, &QComboBox::currentTextChanged, this, &ThenWidgetsLayout::setOperationOptionsBasedOnTarget);
	addWidget(_targetUnit);

	_messagePart2 = new QLabel;
	_messagePart2->setText("to do:");
	addWidget(_messagePart2);

	_operation = new QComboBox(this);
	_operation->setPlaceholderText("operation");
	//_operation->addItem("do 1");
	//_operation->addItem("do 2");
	//_operation->addItem("do 3");
	//_operation->addItem("do 4");
	addWidget(_operation);

	createDeleteButton();
	addStretch(1);
}

void ThenWidgetsLayout::createDeleteButton()
{
	_deleteButton = new QPushButton("-");
	_deleteButton->setStyleSheet(
		"QPushButton {"
		"   background-color: #f44336;"
		"   color: white;"
		"   border: 1px solid #d32f2f;"
		"   border-radius: 1px;"
		"   padding: 1px;"
		"   font-weight: bold;"
		"}"
		"QPushButton:hover {"
		"   background-color: #d32f2f;"
		"}"
	);
	int defaultHeight = _deleteButton->sizeHint().height();
	_deleteButton->setFixedSize(defaultHeight, defaultHeight);
	addWidget(_deleteButton);

	connect(_deleteButton, &QPushButton::clicked, this, [this]() {
		emit requestDelete(this);
		});
}

void ThenWidgetsLayout::addWidget(QWidget* widget)
{
	_layout->addWidget(widget);
}

void ThenWidgetsLayout::addStretch(int stretch)
{
	_layout->addStretch(stretch);
}

std::shared_ptr<Action> ThenWidgetsLayout::data()
{
	std::string targetUnit = _targetUnit->currentText().toStdString();
	std::string message = _operation->currentText().toStdString();



	bool targetUnitHasError = targetUnit.empty();
	bool messageHasError = message.empty();

	if (targetUnitHasError || messageHasError) {
		return nullptr;
	}

	return std::make_shared<Action>(targetUnit, message);
}

void ThenWidgetsLayout::loadFromJson(const nlohmann::json& json)
{
	int targetIndex = _targetUnit->findText(QString::fromStdString(json["target"]));
	if (targetIndex != -1) {
		_targetUnit->setCurrentIndex(targetIndex);
	}

	int operationIndex = _operation->findText(QString::fromStdString(json["message"]));
	if (operationIndex != -1) {
		_operation->setCurrentIndex(operationIndex);
	}
}

nlohmann::json ThenWidgetsLayout::GuiData()
{
	std::string targetUnit = _targetUnit->currentText().toStdString();
	std::string message = _operation->currentText().toStdString();

	if (targetUnit.empty() || message.empty()) {
		return nullptr; 
	}
	return {
		{"target", _targetUnit->currentText().toStdString()},
		{"message", _operation->currentText().toStdString()}
	};
}

unsigned ThenWidgetsLayout::extractIdFromString(const std::string& str)
{
	size_t pos = str.find_last_of(' ');
	if (pos == std::string::npos) {
		throw std::invalid_argument("String format is incorrect.");
	}

	std::string idStr = str.substr(pos + 1);
	return std::stoul(idStr);
}

void ThenWidgetsLayout::setOperationOptionsBasedOnTarget(const QString& text)
{
	_operation->clear();
	auto operationOptions = ControllersManager().getControllersOptions(text.toStdString());
	for (const auto& option : operationOptions)
		_operation->addItem(QString::fromStdString(option));
}