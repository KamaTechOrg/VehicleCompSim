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
	QHBoxLayout* _layout;
	ConditionsEditor* _editor;
	ExplorerBox* _explorer;
};