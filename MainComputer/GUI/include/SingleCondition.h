#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>


#include "ConditionLayoutBase.h"

class SingleCondition : public ConditionLayoutBase
{
	Q_OBJECT

public:
	SingleCondition();
	SingleCondition(const int currentSourceIndex, const int currentTypeIndex, const std::string &currentValidationValue);
	~SingleCondition();

	std::shared_ptr<ConditionBase> logicData() override;
	nlohmann::json GuiData() override;

signals:
	void requestDelete(ConditionLayoutBase* layout);

private:
	QHBoxLayout* _layout;
	QComboBox* _inputSource;
	QComboBox* _conditionType;
	QLineEdit* _validationValue;
	QLabel* _messageFrom;
	QPushButton* _deleteButton;

	void initializeFields();
	//void setBorderColor(QComboBox* comboBox, bool hasError);
	void setBorderColor(QLineEdit* lineEdit, bool hasError);
};