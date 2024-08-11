#include "SingleCondition.h"

#include <stdexcept>
#include <fstream>

#include "ConditionBase.h"
#include "ConditionsFactory.h"

SingleCondition::SingleCondition()
{
	_layout = new QHBoxLayout;

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
	std::vector<std::string> conditionTypes = ConditionsFactory().getConditionTypes();
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

	return ConditionsFactory().createCondition(input, conditionType, validationValue);
}
