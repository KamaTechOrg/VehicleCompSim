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
	std::vector<ConditionsEditor*> _scenariosEditors; // an "editor" for each scenario
	ExplorerBox* _explorer;
	QPushButton* _saveButton;

	void save();
	void initializeGuiFields();

private slots:
	void handleScenarioClicked(int index);
};