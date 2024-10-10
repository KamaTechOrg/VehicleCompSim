#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <memory>
#include "Action.h"
#include "json.hpp"

/**
 * @brief Widget for managing a single action configuration.
 *
 * This class provides a layout for configuring an action, including
 * selection of target units and operations.
 */


class ThenWidgetsLayout : public QWidget
{

	Q_OBJECT

public:
	explicit ThenWidgetsLayout(QWidget* parent = nullptr);

	std::shared_ptr<Action> data();
	void loadFromJson(const nlohmann::json& json);
	nlohmann::json GuiData();

signals:
	void requestDelete(ThenWidgetsLayout* layout);

private:
	QHBoxLayout* _layout;
	QLabel* _messagePart1;
	QComboBox* _targetUnit;
	QLabel* _messagePart2;
	QComboBox* _operation;
	QPushButton* _deleteButton;
	
	void createDeleteButton();
	void addWidget(QWidget* widget);
	void addStretch(int stretch);
	unsigned extractIdFromString(const std::string& str);
	void setOperationOptionsBasedOnTarget(const QString& text);
};