#pragma once

#include <QApplication>
#include <QWidget>

#include "ConditionsEditor.h"
#include "ExplorerBox.h"

class Window : public QWidget
{
public:
	Window();
	~Window();

private:
	QHBoxLayout* _layout;
	ConditionsEditor* _editor;
	ExplorerBox* _explorer;
};