#include "Window.h"

Window::Window()
{
	setWindowTitle("Conditions Editor");

	_layout = new QHBoxLayout;
	_editor = new ConditionsEditor;
	_explorer = new ExplorerBox;
	_layout->addWidget(_explorer);
	_layout->addWidget(_editor);
	setLayout(_layout);
}

Window::~Window()
{
	if (_layout != nullptr)
		delete _layout;
	if (_editor != nullptr)
			delete _editor;
	if (_explorer != nullptr)
		delete _explorer;
}
