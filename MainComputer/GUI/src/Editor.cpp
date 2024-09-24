#include "Editor.h"

#include "JsonLoader.h"

Editor::Editor()
{
	setWindowTitle("Conditions Editor");

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
	std::vector<std::string> scenarioNames = _explorer->scenariosNames();
	for (const auto scenarioName : scenarioNames)
		qInfo() << scenarioName;

	/*bool success = saveLogicDataToJson() && saveGuiDataToJson();
	if (success)
		showSaveSuccessFeedback();
	else
		showSaveFailedFeedback();*/
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
	connect(_explorer, &ExplorerBox::scenarioClicked, this, &Editor::handleScenarioClicked);
	connect(_explorer, &ExplorerBox::scenarioAdded, this, &Editor::handleAddScenario);
	connect(_explorer, &ExplorerBox::scenarioDeleted, this, &Editor::handleDeleteScenario);
	_HLayout->addWidget(_explorer);
}

void Editor::initializeSaveButton()
{
	_saveButton = new QPushButton("save");
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