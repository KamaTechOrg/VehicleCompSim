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
	
	void save();

private:
	void showSaveFeedback(bool success);
	void loadDataFromJson(const std::string& filename);


	QVBoxLayout* _ConditionsEditorLayout;
	ConditionsGroup* _conditionsGroup;
	ThenGroupBox* _thenGroupBox;
	QPushButton* _saveButton;
};