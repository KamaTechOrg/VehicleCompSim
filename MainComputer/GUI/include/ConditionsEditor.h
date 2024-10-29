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
    ConditionsEditor(nlohmann::json &_guiData, nlohmann::json &_logicData);
	
	nlohmann::json getLogicDataAsJson();
	nlohmann::json getGuiDataAsJson();
	void setView(nlohmann::json jsonData);

private:
	void loadGuiDataFromJson();

	QVBoxLayout* _ConditionsEditorLayout;
	ConditionsGroup* _conditionsGroup;
	ThenGroupBox* _actionGroupBox;

    nlohmann::json &guiData, &logicData;
};
