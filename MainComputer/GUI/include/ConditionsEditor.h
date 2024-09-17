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
	
	void setView(nlohmann::json jsonData);
	void save();

private:
	void showSaveFeedback(bool success);
	void loadGuiDataFromJson();
	void saveLogicDataToJson();
	void saveGuiDataToJson();

	QVBoxLayout* _ConditionsEditorLayout;
	ConditionsGroup* _conditionsGroup;
	ThenGroupBox* _actionGroupBox;
	QPushButton* _saveButton;
};