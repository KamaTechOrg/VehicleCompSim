#pragma once

#include <QApplication>
#include <QWidget>

#include "ConditionsEditor.h"
#include "ExplorerBox.h"

/**
 * @brief Main editor for managing scenarios and their conditions.
 *
 * This class provides a graphical interface for editing scenarios,
 * allowing users to add, delete, and save conditions for each scenario.
 */

class Editor : public QWidget
{
	Q_OBJECT

public:
	Editor();
	~Editor();

private:
	QVBoxLayout* _VLayout;
	QHBoxLayout* _HLayout;
	QVBoxLayout* _conditionsEditorLayout;
	std::vector<ConditionsEditor*> _scenariosEditors; // an "editor" for each scenario
	ExplorerBox* _explorer;
	QPushButton* _saveButton;

	void save();
	bool saveLogicDataToJson();
	bool saveGuiDataToJson();
	void showSaveSuccessFeedback();
	void showSaveFailedFeedback();
	void initializeGuiFields();
	void initializeScenariosExplorer();
	void initializeSaveButton();
	void initializeScenriosEditors();

private slots:
	void handleScenarioClicked(int index);
	void handleAddScenario();
	void handleDeleteScenario(int index);
};