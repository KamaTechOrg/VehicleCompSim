#ifndef RESOURCE_DIR
#define RESOURCE_DIR ""
#endif

#include "Editor.h"
#include "JsonLoader.h"

#include <QDebug>
#include <conditionsManager.h>

Editor::Editor()
{
	setWindowTitle("Conditions Editor");
	setWindowIcon(QIcon(QString(RESOURCE_DIR) + "/cond.png"));

	nlohmann::json::array_t guiData = JsonLoader().loadGuiData();
	for (nlohmann::json scenario : guiData)
	{
		ConditionsEditor* scenarioEditor = new ConditionsEditor;
		scenarioEditor->setView(scenario);
		scenarioEditor->hide();
		_scenariosEditors.push_back(scenarioEditor);
	}
	initializeGuiFields();
}

Editor::~Editor()
{
	if (_HLayout != nullptr)
		delete _HLayout;
	if (_VLayout != nullptr)
		delete _VLayout;
	for (auto editor : _scenariosEditors)
		if (editor != nullptr)
			delete editor;
	if (_explorer != nullptr)
		delete _explorer;
}

void Editor::save()
{
	bool success = false;
	success = saveLogicDataToJson();
	success &= saveGuiDataToJson();
	if (success)
		showSaveSuccessFeedback();
	else
		showSaveFailedFeedback();
}

bool Editor::saveLogicDataToJson()
{
	// get the data from the gui fields
	nlohmann::json::array_t jsonData;
	for (auto scenario : _scenariosEditors)
	{
		nlohmann::json current = scenario->getLogicDataAsJson();
		if (!current.contains("conditions") && !current.contains("actions"))
			return false;
		if (!current["actions"].is_array() || current["actions"].empty())
			return false;
		jsonData.push_back(current);
	}
	
	// save to JSON file
	JsonLoader().saveConditionsLogic(jsonData);
	
	// load the "backend" engine of the main computer with the new conditions
	ConditionsManager().loadFromJson();

	return true;
}

bool Editor::saveGuiDataToJson()
{
	std::vector<std::string> scenarioNames = _explorer->scenariosNames();

	if (scenarioNames.size() != _scenariosEditors.size())
	{
		qWarning() << "scenarioNames.size() = " << scenarioNames.size() << "_scenariosEditors.size() = " << "_scenariosEditors.size()";
		return false;
	}

	nlohmann::json::array_t jsonData;
	for (int i = 0; i < _scenariosEditors.size(); i++)
	{
		nlohmann::json current = _scenariosEditors[i]->getGuiDataAsJson();
		current["scenarioName"] = scenarioNames[i];
		jsonData.push_back(current);
	}

	JsonLoader().saveGuiData(jsonData);
	return true;
}

void Editor::showSaveSuccessFeedback()
{
	_saveButton->setStyleSheet("background-color: #4CAF50;"); // Green
	_saveButton->setText("success");
	QTimer::singleShot(3000, [this]() {
	    _saveButton->setStyleSheet("");
	    _saveButton->setText("save");
	    });
}

void Editor::showSaveFailedFeedback()
{
	_saveButton->setStyleSheet("background-color: #F44336;"); // Red
	_saveButton->setText("failed");
	QTimer::singleShot(3000, [this]() {
	    _saveButton->setStyleSheet("");
	    _saveButton->setText("save");
	    });
}

void Editor::initializeGuiFields()
{
	_VLayout = new QVBoxLayout;
	_HLayout = new QHBoxLayout;
	
	initializeScenariosExplorer();
	initializeScenriosEditors();
	_HLayout->addStretch(1);
	_VLayout->addLayout(_HLayout);
	initializeSaveButton();

	setLayout(_VLayout);
	resize(700, 200);
}

void Editor::initializeScenariosExplorer()
{
	_explorer = new ExplorerBox;

	std::vector<std::string> names;
	nlohmann::json::array_t guiData = JsonLoader().loadGuiData();
	for (nlohmann::json scenario : guiData)
	{
		if (scenario.contains("scenarioName"))
			names.push_back(scenario["scenarioName"]);
		else
			names.push_back("invalid data");
	}
	_explorer->setScenariosList(names);

	connect(_explorer, &ExplorerBox::scenarioClicked, this, &Editor::handleScenarioClicked);
	connect(_explorer, &ExplorerBox::scenarioAdded, this, &Editor::handleAddScenario);
	connect(_explorer, &ExplorerBox::scenarioDeleted, this, &Editor::handleDeleteScenario);

	_HLayout->addWidget(_explorer);
}

void Editor::initializeSaveButton()
{
	_saveButton = new QPushButton("save");
	QFont font = _saveButton->font();
	font.setBold(true);
	_saveButton->setFont(font);
	connect(_saveButton, &QPushButton::clicked, this, &Editor::save);
	_VLayout->addWidget(_saveButton);
}

void Editor::initializeScenriosEditors()
{
	_conditionsEditorLayout = new QVBoxLayout;
	for (auto editor : _scenariosEditors)
		_conditionsEditorLayout->addWidget(editor);
	_HLayout->addLayout(_conditionsEditorLayout);
}

void Editor::handleScenarioClicked(int index)
{
	for (auto editor : _scenariosEditors)
		editor->hide();

	if (index < _scenariosEditors.size())
		_scenariosEditors.at(index)->show();
}

void Editor::handleAddScenario()
{
	ConditionsEditor* newScenarioEditor = new ConditionsEditor;
	_conditionsEditorLayout->addWidget(newScenarioEditor);
	newScenarioEditor->hide();
	_scenariosEditors.push_back(newScenarioEditor);
}

void Editor::handleDeleteScenario(int index)
{
	if (index < 0 || index >= _scenariosEditors.size())
		return;

	ConditionsEditor* editorToDelete = _scenariosEditors.at(index);
	editorToDelete->hide();
	_conditionsEditorLayout->removeWidget(editorToDelete);
	delete editorToDelete;
	_scenariosEditors.erase(_scenariosEditors.begin() + index);
}