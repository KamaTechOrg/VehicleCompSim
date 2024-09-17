#include "ExplorerBox.h"

#include <QStringListModel>
#include "JsonLoader.h"

ExplorerBox::ExplorerBox(ConditionsEditor* editorReference)
	: QGroupBox("Explorer"), _editorReference(editorReference)
{
	_scenariosList = new QListView;

	QStringListModel* model = new QStringListModel(this);
	QStringList scenarioList;
	scenarioList << "scenario 1" << "scenario 2" << "scenario 3"
		<< "scenario 4" << "scenario 5";

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
	qDebug() << itemText << " clicked";
	nlohmann::json jsonData = JsonLoader().loadGuiData();
	_editorReference->setView(jsonData);
}