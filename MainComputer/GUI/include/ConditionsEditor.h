#pragma once

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

#include "ConditionsBox.h"
#include "ThenGroupBox.h"

class ConditionsEditor : public QGroupBox
{
public:
	ConditionsEditor();
	
	void save();

private:
	QVBoxLayout* _ConditionsEditorLayout;
	ConditionsBox* _ConditionsBox;
	ThenGroupBox* _thenGroupBox;
	QPushButton* _saveButton;

};