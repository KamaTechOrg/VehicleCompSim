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

	model->setStringList(scenarioList);
	_scenariosList->setModel(model);

	_layout = new QVBoxLayout;
	_layout->addWidget(_scenariosList);
	setLayout(_layout);
}
