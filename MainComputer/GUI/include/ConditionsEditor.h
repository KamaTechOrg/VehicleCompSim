#pragma once

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include "ConditionsGroup.h"
#include "ThenGroupBox.h"

/**
 * @brief Editor for managing and displaying conditions in a graphical interface.
 *
 * This class provides a user interface for editing conditions,
 * allowing users to view and manipulate logic and GUI data.
 */

class ConditionsEditor : public QGroupBox
{
public:
	ConditionsEditor();
	
	nlohmann::json getLogicDataAsJson();
	nlohmann::json getGuiDataAsJson();
	void setView(nlohmann::json jsonData);

private:
	void loadGuiDataFromJson();

	QVBoxLayout* _ConditionsEditorLayout;
	ConditionsGroup* _conditionsGroup;
	ThenGroupBox* _actionGroupBox;

};