#pragma once

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ConditionsEditor.h"
#include "ExplorerBox.h"
#include "Window.h"
#include "ConditionsManager.h"


class Gui : public QWidget
{
	Q_OBJECT

public:
	Gui();
	~Gui();

private slots:
	void onOpenEditor();
	void onStartStopEditor();

private:
	QHBoxLayout* _mainLayout;
	QVBoxLayout* _buttonLayout;
	QPushButton* _openEditorButton;
	QPushButton* _startStopButton;
	QPushButton* _statusButton;
	bool _editorRunning;
	Window editor;
//	ConditionsManager manager;
};