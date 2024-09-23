#include "Editor.h"

#include "JsonLoader.h"

Editor::Editor()
{
	setWindowTitle("Conditions Editor");

	nlohmann::json::array_t guiData = JsonLoader().loadGuiData();
	for (const auto scenario : guiData)
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
	
	_explorer = new ExplorerBox();
	connect(_explorer, &ExplorerBox::scenarioClicked, this, &Editor::handleScenarioClicked);

	_saveButton = new QPushButton("save");
	connect(_saveButton, &QPushButton::clicked, this, &Editor::save);

	_HLayout->addWidget(_explorer);
	for (auto editor : _scenariosEditors)
		_HLayout->addWidget(editor);

	_VLayout->addLayout(_HLayout);
	_VLayout->addWidget(_saveButton);

	setLayout(_VLayout);
	resize(700, 200);
}

void Editor::handleScenarioClicked(int index)
{
	for (auto editor : _scenariosEditors)
		editor->hide();

	_scenariosEditors.at(index)->show();
}