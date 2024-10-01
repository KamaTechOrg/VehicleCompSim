#pragma once

#include <vector>
#include <QGroupBox>
#include <QListView>
#include <QVBoxLayout>

#include "ConditionsEditor.h"

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