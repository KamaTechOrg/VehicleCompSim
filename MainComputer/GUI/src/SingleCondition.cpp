#include "SingleCondition.h"

#include <fstream>
#include "SimpleCondition.h"

SingleCondition::SingleCondition()
	: _andOrButton(nullptr)
{
	_SingleCondition = new QHBoxLayout;

	_inputSource = new QComboBox();
	_inputSource->setPlaceholderText("input source");
	_inputSource->addItem("sensor 1");
	_inputSource->addItem("sensor 2");
	_inputSource->addItem("sensor 3");
	_SingleCondition->addWidget(_inputSource);

	_conditionType = new QComboBox();
	_conditionType->setPlaceholderText("condition");
	_conditionType->addItem("greater then");
	_conditionType->addItem("smaller then");
	_conditionType->addItem("equels to");
	_SingleCondition->addWidget(_conditionType);

	_validationValue = new QLineEdit();
	_validationValue->setPlaceholderText("validation value");
	_SingleCondition->addWidget(_validationValue);

	_deleteButton = new QPushButton("-");
	int defaultHeight = _deleteButton->sizeHint().height();
	_deleteButton->setFixedSize(defaultHeight, defaultHeight);
	_SingleCondition->addWidget(_deleteButton);
	
	connect(_deleteButton, &QPushButton::clicked, this, [this]() {
		emit requestDelete(this);
	});

	_SingleCondition->addStretch(1);
	addLayout(_SingleCondition);

	addStretch(1);
}

SingleCondition::~SingleCondition()
{
	if (_inputSource)
		delete _inputSource;
	if (_conditionType)
		delete _conditionType;
	if (_validationValue)
		delete _validationValue;
	if (_deleteButton)
		delete _deleteButton;
	if (_andOrButton != nullptr)
		delete _andOrButton;
}

void SingleCondition::setAndOrButton(bool And)
{
	if (!_andOrButton)
	{
		_andOrButton = new QPushButton(And == true ? "and" : "or");
		int defaultHeight = _andOrButton->sizeHint().height();
		_andOrButton->setFixedSize(defaultHeight * 3, defaultHeight);

		insertWidget(0, _andOrButton);

		connect(_andOrButton, &QPushButton::clicked, this, &SingleCondition::andOrButtonSwitch);
	}
	else
	{
		_andOrButton->setText(And == true ? "and" : "or");
	}
}

void SingleCondition::andOrButtonSwitch()
{
	QString text = _andOrButton->text();
	if (text == "or")
		_andOrButton->setText("and");
	else
		_andOrButton->setText("or");
}

ConditionBase* SingleCondition::data()
{
	SimpleCondition* condition = new SimpleCondition(
		_inputSource->currentText().toStdString(),
		_conditionType->currentText().toStdString(),
		_validationValue->text().toStdString());

	return condition;
}

ConditionLayoutBase::conditionType SingleCondition::getConditionType()
{
	if (_andOrButton != nullptr)
	{
		if (_andOrButton->text().toStdString() == "and")
			return conditionType::And;
		else if (_andOrButton->text().toStdString() == "or")
			return conditionType::Or;
	}
	return conditionType::Null;
}

void SingleCondition::deleteAndOrButton()
{
	_andOrButton->deleteLater();
	delete _andOrButton;
	_andOrButton = nullptr;
}
