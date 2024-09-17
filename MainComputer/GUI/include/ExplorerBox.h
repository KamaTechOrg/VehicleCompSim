#pragma once

#include <QGroupBox>
#include <QListView>
#include <QVBoxLayout>

#include "ConditionsEditor.h"

class ExplorerBox : public QGroupBox
{
public:
	ExplorerBox(ConditionsEditor* editorReference);

private:
	ConditionsEditor* _editorReference;
	QVBoxLayout* _layout;
	QListView* _scenariosList;

	void onItemClicked(const QModelIndex& index);
};