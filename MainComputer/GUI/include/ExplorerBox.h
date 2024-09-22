#pragma once

#include <vector>
#include <QGroupBox>
#include <QListView>
#include <QVBoxLayout>

#include "ConditionsEditor.h"

class ExplorerBox : public QGroupBox
{
public:
	ExplorerBox(ConditionsEditor* editorReference);

	std::vector<std::string> scenariosNames() const;

private:
	ConditionsEditor* _editorReference;
	QVBoxLayout* _layout;
	QListView* _scenariosList;

	void onItemClicked(const QModelIndex& index);
	void showContextMenu(const QPoint& pos);
	void renameSelectedItem();
	void deleteSelectedItem();
	void addNewItem();
};