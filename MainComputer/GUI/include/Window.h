#pragma once

#include <QApplication>
#include <QWidget>

#include "ConditionsEditor.h"
#include "ExplorerBox.h"

/**
 * @brief Main application window for managing conditions and actions.
 *
 * This class serves as the primary interface for users to edit conditions
 */


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