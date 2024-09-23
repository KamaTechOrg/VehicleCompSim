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

signals:
	void scenarioClicked(int index);

private:
	QVBoxLayout* _layout;
	QListView* _scenariosList;

	void onItemClicked(const QModelIndex& index);
	void showContextMenu(const QPoint& pos);
	void renameSelectedItem();
	void deleteSelectedItem();
	void addNewItem();
};