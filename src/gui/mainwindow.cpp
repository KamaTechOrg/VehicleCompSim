#include "mainwindow.h"
#include "customwidget.h"
#include <QGraphicsView>
#include <QToolBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QUuid>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_scene(std::make_unique<CustomScene>()) {
    m_view = new QGraphicsView(m_scene.get());

    setupToolBar();

    auto mainWidget = new QWidget(this);
    auto layout = new QHBoxLayout(mainWidget);
    layout->addWidget(m_toolBar);
    layout->addWidget(m_view);

    setCentralWidget(mainWidget);

    auto toolBar = addToolBar("Tools");
    toolBar->addAction("Save", [this]() { saveLayout(); });
    toolBar->addAction("Load", [this]() { loadLayout(); });
}

void MainWindow::setupToolBar() {
    m_toolBar = new QToolBar("Shapes", this);
    m_toolBar->setOrientation(Qt::Vertical);

    auto rItemWidget = new CustomWidget("SensorItem", this);
    m_toolBar->addWidget(rItemWidget);

    auto busWidget = new CustomWidget("ConnectorItem", this);
    m_toolBar->addWidget(busWidget);

    addToolBar(Qt::LeftToolBarArea, m_toolBar);
}

void MainWindow::saveLayout() {
    // QString fileName = QFileDialog::getSaveFileName(this, "Save Layout", "", "JSON Files (*.json)");
    // if (fileName.isEmpty()) return;

    // QJsonArray itemsArray;
    // for (auto item : m_scene->items()) {
    //     if (auto customItem = dynamic_cast<CustomItem*>(item)) {
    //         QJsonObject itemObject;
    //         itemObject["type"] = customItem->type();
    //         itemObject["id"] = customItem->id();
    //         itemObject["x"] = customItem->x();
    //         itemObject["y"] = customItem->y();
    //         itemsArray.append(itemObject);
    //     }
    // }

    // QJsonDocument doc(itemsArray);
    // QFile file(fileName);
    // if (file.open(QIODevice::WriteOnly)) {
    //     file.write(doc.toJson());
    // }
}

void MainWindow::loadLayout() {
    // QString fileName = QFileDialog::getOpenFileName(this, "Load Layout", "", "JSON Files (*.json)");
    // if (fileName.isEmpty()) return;

    // QFile file(fileName);
    // if (file.open(QIODevice::ReadOnly)) {
    //     QByteArray data = file.readAll();
    //     QJsonDocument doc(QJsonDocument::fromJson(data));
    //     QJsonArray itemsArray = doc.array();

    //     m_scene->clear();
    //     for (const auto& value : itemsArray) {
    //         QJsonObject obj = value.toObject();
    //         QString type = obj["type"].toString();
    //         QString id = obj["id"].toString();
    //         qreal x = obj["x"].toDouble();
    //         qreal y = obj["y"].toDouble();

    //         auto item = std::make_unique<CustomItem>(type, id);
    //         item->setPos(x, y);

    //         m_scene->addItem(item.release());
    //     }

    //     // Update the entire scene to ensure items are rendered
    //     m_scene->update();
    // }
}
