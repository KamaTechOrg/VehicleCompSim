#include "ConditionBase.h"
#include "ConditionsFactory.h"
#include "SingleCondition.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QString>
#include <QPushButton>


SingleCondition::SingleCondition()
{
	_layout = new QHBoxLayout;

	_messageFrom = new QLineEdit();
	_messageFrom->setPlaceholderText("message from");
	_layout->addWidget(_messageFrom);

	_inputSource = new QComboBox();
	_inputSource->setPlaceholderText("input source");
	_inputSource->addItem("id 1");
	_inputSource->addItem("id 2");
	_inputSource->addItem("id 3");
	_inputSource->addItem("id 4");
	_inputSource->addItem("id 5");
	_layout->addWidget(_inputSource);

	_conditionType = new QComboBox();
	_conditionType->setPlaceholderText("condition");
	std::vector<std::string> conditionTypes = ConditionsFactory().getSimpleConditionTypes();
	for (const auto& type : conditionTypes)
		_conditionType->addItem(QString(type.c_str()));

	_layout->addWidget(_conditionType);

	_validationValue = new QLineEdit();
	_validationValue->setPlaceholderText("validation value");
	_layout->addWidget(_validationValue);

	_deleteButton = new QPushButton("-");
	int defaultHeight = _deleteButton->sizeHint().height();
	_deleteButton->setFixedSize(defaultHeight, defaultHeight);
	_layout->addWidget(_deleteButton);

	connect(_deleteButton, &QPushButton::clicked, this, [this]() {
		emit requestDelete(this);
		});

	_layout->addStretch(1);
	addLayout(_layout);

	addStretch(1);
}

SingleCondition::~SingleCondition()
{
	if (_inputSource != nullptr)
		delete _inputSource;
	if (_conditionType != nullptr)
		delete _conditionType;
	if (_validationValue != nullptr)
		delete _validationValue;
	if (_messageFrom != nullptr)
		delete _messageFrom;
	if (_deleteButton != nullptr)
		delete _deleteButton;
	if (_layout != nullptr)
		delete _layout;
}

std::string to_string(const QString& qstr) {
	return qstr.toStdString();
}

std::shared_ptr<ConditionBase> SingleCondition::data()
{
	std::string input = _inputSource->currentText().toStdString();
	std::string conditionType = _conditionType->currentText().toStdString();
	std::string validationValue = _validationValue->text().toStdString();

	bool inputHasError = input.empty();
	bool conditionTypeHasError = conditionType.empty();
	bool validationValueHasError = validationValue.empty();

	setBorderColor(_inputSource, inputHasError);
	setBorderColor(_conditionType, conditionTypeHasError);
	setBorderColor(_validationValue, validationValueHasError);

	if (input.empty() || conditionType.empty() || validationValue.empty()) {
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("Validation Error"));
		msgBox.setText(tr("Please make sure all fields are filled out."));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
		return nullptr;
	}

	return ConditionsFactory().createSimpleCondition(input, conditionType, validationValue);
}


void SingleCondition::setBorderColor(QComboBox* comboBox, bool hasError)
{
	comboBox->setStyleSheet(
		hasError
		? "QComboBox { border: 2px solid red; } QComboBox::drop-down { border: none; }"
		: "QComboBox { border: none; }"
	);
}

void SingleCondition::setBorderColor(QLineEdit* lineEdit, bool hasError)
{
	lineEdit->setStyleSheet(hasError ? "border: 2px solid red;" : "");
}