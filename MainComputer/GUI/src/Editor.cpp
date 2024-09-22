#include "Editor.h"

Editor::Editor()
{
	setWindowTitle("Conditions Editor");

	_VLayout = new QVBoxLayout;
	_HLayout = new QHBoxLayout;
	_editor = new ConditionsEditor;
	_explorer = new ExplorerBox(_editor);
	_saveButton = new QPushButton("save");

	connect(_saveButton, &QPushButton::clicked, this, &Editor::save);

	_HLayout->addWidget(_explorer);
	_HLayout->addWidget(_editor);

	_VLayout->addLayout(_HLayout);
	_VLayout->addWidget(_saveButton);

	setLayout(_VLayout);
	resize(700, 200);
}

Editor::~Editor()
{
	if (_HLayout != nullptr)
		delete _HLayout;
	if (_VLayout != nullptr)
		delete _VLayout;
	if (_editor != nullptr)
			delete _editor;
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