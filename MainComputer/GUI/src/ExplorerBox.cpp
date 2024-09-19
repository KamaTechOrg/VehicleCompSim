#include "ExplorerBox.h"

#include <QStringListModel>
#include <QMenu>
#include "JsonLoader.h"

ExplorerBox::ExplorerBox(ConditionsEditor* editorReference)
	: QGroupBox("Explorer"), _editorReference(editorReference)
{
	this->setFixedWidth(250);

	_scenariosList = new QListView;

	QStringListModel* model = new QStringListModel(this);
	QStringList scenarioList;
	scenarioList << "New Scenario";

	model->setStringList(scenarioList);
	_scenariosList->setModel(model);
	connect(_scenariosList, &QListView::clicked, this, &ExplorerBox::onItemClicked);
	_scenariosList->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
	_scenariosList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(_scenariosList, &QListView::customContextMenuRequested, this, &ExplorerBox::showContextMenu);

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

void ExplorerBox::showContextMenu(const QPoint& pos)
{
	// Create a context menu
	QMenu contextMenu;
	QAction* addAction = new QAction("Add", this);
	QAction* renameAction = new QAction("Rename", this);
	QAction* deleteAction = new QAction("Delete", this);

	connect(addAction, &QAction::triggered, this, &ExplorerBox::addNewItem);
	connect(renameAction, &QAction::triggered, this, &ExplorerBox::renameSelectedItem);
	connect(deleteAction, &QAction::triggered, this, &ExplorerBox::deleteSelectedItem);

	contextMenu.addAction(addAction);
	contextMenu.addAction(renameAction);
	contextMenu.addAction(deleteAction);

	// Display the context menu at the requested position
	contextMenu.exec(_scenariosList->viewport()->mapToGlobal(pos));
}

void ExplorerBox::renameSelectedItem()
{
	QModelIndex index = _scenariosList->currentIndex();
	if (index.isValid()) {
		_scenariosList->edit(index);
	}
}

void ExplorerBox::deleteSelectedItem()
{
	QModelIndex index = _scenariosList->currentIndex();
	if (index.isValid()) {
		QStringListModel* model = qobject_cast<QStringListModel*>(_scenariosList->model());
		if (model) {
			model->removeRow(index.row());
		}
	}
}

void ExplorerBox::addNewItem()
{
	QModelIndex index = _scenariosList->currentIndex();
	if (index.isValid()) {
		QStringListModel* model = qobject_cast<QStringListModel*>(_scenariosList->model());
		if (model) {
			QStringList scenariosList = model->stringList();
			scenariosList << "New Scenario";
			model->setStringList(scenariosList);
		}
	}
}