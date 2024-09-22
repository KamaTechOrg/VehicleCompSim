#pragma once

#include <QApplication>
#include <QWidget>

#include "ConditionsEditor.h"
#include "ExplorerBox.h"

class Editor : public QWidget
{
public:
	Editor();
	~Editor();

private:
	QVBoxLayout* _VLayout;
	QHBoxLayout* _HLayout;
	ConditionsEditor* _editor;
	ExplorerBox* _explorer;
	QPushButton* _saveButton;

	void save();
};