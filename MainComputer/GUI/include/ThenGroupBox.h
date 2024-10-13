#pragma once

#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include "ThenWidgetsLayout.h"
#include "Action.h"

/**
 * @brief Group box for managing actions in the user interface.
 *
 * This class provides a layout for displaying and managing action widgets,
 * allowing users to add or remove actions as needed.
 */

class ThenGroupBox : public QGroupBox
{
public:
	ThenGroupBox(QWidget* parent = nullptr);

	std::vector<std::shared_ptr<Action>> data();
	void setView(const nlohmann::json& json);
	nlohmann::json GuiData();

private:
	QVBoxLayout* _mainLayout;
	std::vector<ThenWidgetsLayout*> _actionLayouts;
	QPushButton* _addActionButton;

	void addActionLayout();
	void removeActionLayout(ThenWidgetsLayout* layout);
};