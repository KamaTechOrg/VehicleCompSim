#include "ExplorerBox.h"

#include <QStringListModel>

ExplorerBox::ExplorerBox()
	: QGroupBox("Explorer")
{
	_scenariosList = new QListView;

	QStringListModel* model = new QStringListModel(this);
	QStringList scenarioList;
	scenarioList << "scenario 1" << "scenario 2" << "scenario 3"
		<< "scenario 4" << "scenario 5";
	scenarioList << "scenario 6";

	model->setStringList(scenarioList);
	_scenariosList->setModel(model);
	connect(_scenariosList, &QListView::clicked, this, &ExplorerBox::onItemClicked);

	_layout = new QVBoxLayout;
	_layout->addWidget(_scenariosList);
	setLayout(_layout);
}

void ExplorerBox::onItemClicked(const QModelIndex& index)
{
	QString itemText = index.data().toString();

	qInfo() << itemText << " clicked";
}