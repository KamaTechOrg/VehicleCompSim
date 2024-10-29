#pragma once

#include <QApplication>
#include <QWidget>

#include "ConditionsEditor.h"
#include "ExplorerBox.h"

class Editor : public QWidget
{
	Q_OBJECT

public:
    Editor(nlohmann::json &_guiData, nlohmann::json &_logicData);
	~Editor();

    nlohmann::json &guiData, &logicData;
signals:
    void dataChanged();

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
