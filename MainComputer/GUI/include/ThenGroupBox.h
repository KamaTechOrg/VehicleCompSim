#pragma once

#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include "ThenWidgetsLayout.h"
#include "Action.h"

class ThenGroupBox : public QGroupBox
{
public:
	ThenGroupBox(QWidget* parent = nullptr);

	std::vector<std::shared_ptr<Action>> data();
	void loadFromJson(const nlohmann::json& json);
	nlohmann::json GuiData();

private:
	QVBoxLayout* _mainLayout;
	std::vector<ThenWidgetsLayout*> _actionLayouts;
	QPushButton* _addActionButton;

	void addActionLayout();
	void removeActionLayout(ThenWidgetsLayout* layout);
};