#pragma once

#include <QGroupBox>
#include <QListView>
#include <QVBoxLayout>

class ExplorerBox : public QGroupBox
{
public:
	ExplorerBox();

private:
	QVBoxLayout* _layout;
	QListView* _scenariosList;

	void onItemClicked(const QModelIndex& index);
};