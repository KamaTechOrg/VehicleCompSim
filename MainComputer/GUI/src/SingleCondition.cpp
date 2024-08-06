#include "SingleCondition.h"
#include "GreaterThanCondition.h"
#include "SmallerThanCondition.h"
#include "EqualsToCondition.h"
#include "StartsWithCondition.h"
#include "EndsWithCondition.h"
#include "ContainsCondition.h"
#include "SimpleCondition.h"
#include <stdexcept>
#include <fstream>
#include <unordered_map>
#include <functional>

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
	_conditionType->addItem("greater then");
	_conditionType->addItem("smaller then");
	_conditionType->addItem("equels to");
	_conditionType->addItem("starts with");
	_conditionType->addItem("ends with");
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
	std::string conditionType = to_string(_conditionType->currentText());
	std::string validationValue = _validationValue->text().toStdString();

	static const std::unordered_map<std::string, std::function<std::shared_ptr<ConditionBase>()>> conditionFactory = {
		{"greater than", [input, validationValue]() {
			return std::make_shared<GreaterThanCondition>(input, validationValue);
		}},
		{"smaller than", [input, validationValue]() {
			return std::make_shared<SmallerThanCondition>(input, validationValue);
		}},
		{"equals to", [input, validationValue]() {
			return std::make_shared<EqualsToCondition>(input, validationValue);
		}},
		{"starts with", [input, validationValue]() {
			return std::make_shared<StartsWithCondition>(input, validationValue);
		}},
		{"ends with", [input, validationValue]() {
			return std::make_shared<EndsWithCondition>(input, validationValue);
		}},
		{"contains", [input, validationValue]() {
			return std::make_shared<ContainsCondition>(input, validationValue);
		}}
	};

	auto it = conditionFactory.find(conditionType);
	if (it != conditionFactory.end()) {
		return it->second();
	}
	else {
		throw std::runtime_error("Invalid condition type: " + conditionType);
	}
}
