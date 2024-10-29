#include "Gui.h"

Gui::Gui() : editor(guiData, logicData), _editorRunning(false)
{
	setWindowTitle("Menu");

	_mainLayout = new QHBoxLayout(this);
	_buttonLayout = new QVBoxLayout();

	_openEditorButton = new QPushButton("Open Editor", this);
	_startStopButton = new QPushButton("Start/stop Manager", this);
	_statusButton = new QPushButton("Not Running", this);

	connect(_openEditorButton, &QPushButton::clicked, this, &Gui::onOpenEditor);
	connect(_startStopButton, &QPushButton::clicked, this, &Gui::onStartStopComputer);

	_buttonLayout->addWidget(_openEditorButton);
	_buttonLayout->addWidget(_startStopButton);
	_buttonLayout->addWidget(_statusButton);


	_mainLayout->addLayout(_buttonLayout);
	setLayout(_mainLayout);

	resize(400, 200);

	_statusButton->setStyleSheet("background-color: red; color: white;");

}

Gui::~Gui()
{
	delete _openEditorButton;
	delete _startStopButton;
	delete _statusButton;
	delete _buttonLayout;
	delete _mainLayout;
}

void Gui::onOpenEditor()
{
	editor.show();
}

void Gui::onStartStopComputer()
{
	if (manager.isRunning())
	{
		manager.stop();
		_statusButton->setText("Not Running");
		_statusButton->setStyleSheet("background-color: red; color: white;");
	}
	else
	{
        manager.jsonData = logicData;
     	manager.run();
		_statusButton->setText("Running");
		_statusButton->setStyleSheet("background-color: green; color: white;");
	}
}

