#pragma once

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "ConditionsGroup.h"
#include "ThenGroupBox.h"

class ConditionsEditor : public QGroupBox
{
public:
	ConditionsEditor();
	
	void save();

private:
	QVBoxLayout* _ConditionsEditorLayout;
	ConditionsGroup* _conditionsGroup;
	ThenGroupBox* _thenGroupBox;
	QPushButton* _saveButton;
};