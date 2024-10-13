#pragma once

#include <vector>
#include <QGroupBox>
#include <QListView>
#include <QVBoxLayout>

#include "ConditionsEditor.h"

/**
 * @brief A group box for exploring and managing scenarios.
 *
 * This class provides a user interface for listing scenarios,
 * allowing users to select, add, or delete scenarios.
 */


class ExplorerBox : public QGroupBox
{
	Q_OBJECT

public:
	ExplorerBox();

	std::vector<std::string> scenariosNames() const;
	void setScenariosList(std::vector<std::string> scenariosNames);

signals:
	void scenarioClicked(int index);
	void scenarioAdded();
	void scenarioDeleted(int index);

private:
	QVBoxLayout* _layout;
	QListView* _scenariosList;
	QStringList _scenariosListItems;

	void onItemClicked(const QModelIndex& index);
	void showContextMenu(const QPoint& pos);
	void renameSelectedItem();
	void deleteSelectedItem();
	void addNewItem();
};