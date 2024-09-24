#pragma once

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include "ConditionsGroup.h"
#include "ThenGroupBox.h"

class ConditionsEditor : public QGroupBox
{
public:
	ConditionsEditor();
	
	nlohmann::json getLogicDataAsJson();
	nlohmann::json getGuiDataAsJson();
	void setView(nlohmann::json jsonData);

private:
	void showSaveSuccessFeedback();
	void showSaveFailedFeedback();
	void loadGuiDataFromJson();

	QVBoxLayout* _ConditionsEditorLayout;
	ConditionsGroup* _conditionsGroup;
	ThenGroupBox* _actionGroupBox;

	//void save();
};