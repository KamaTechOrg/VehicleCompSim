#include "mainwindow.h"
#include "customwidget.h"
#include "qpushbutton.h"
#include <QGraphicsView>
#include <QToolBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QUuid>
#include "customwidget.h"
#include "client/websocketclient.h"

#include <QTimeEdit>
#include <QRect>
#include <bson/bson.h>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_scene(new CustomScene()) {
    m_view = new QGraphicsView(m_scene);

    setupToolBar();

    auto mainWidget = new QWidget(this);
    auto layout = new QHBoxLayout(mainWidget);
    layout->addWidget(m_toolBar);
    layout->addWidget(m_view);

    setCentralWidget(mainWidget);

    auto toolBar = addToolBar("Tools");
    toolBar->addAction("background", [this]() { background_Layout(); });
    toolBar->addAction("Save", [this]() { saveLayout(); });
    toolBar->addAction("Load", [this]() { loadLayout(); });

    WebSocketClient& client = WebSocketClient::getInstance();
    client.setScene(m_scene);
    connect(&client, &WebSocketClient::connectionStatusChanged, this, &MainWindow::onConnectionStatusChanged);
      
    setupRunService();
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

void MainWindow::onConnectionStatusChanged(bool connected)
{
  
}

void MainWindow::setupRunService()
{
    runService.setScene(m_scene);

    startBtn = new QPushButton("start", m_toolBar);
    m_toolBar->addWidget(startBtn);
    stopBtn = new QPushButton("stop", m_toolBar);
    m_toolBar->addWidget(stopBtn);
    timer = new QTimeEdit(m_toolBar);
    timer->setDisplayFormat("hh:mm:ss");
    timer->setFixedSize(120, 30);
    timer->setCurrentSection(QDateTimeEdit::MinuteSection);
    m_toolBar->addWidget(timer);

    m_toolbar_blocker = new ActionsBlocker(m_toolBar);
    m_scene_blocker = new ActionsBlocker(m_view);
    m_scene_blocker->transparency(0.0);
    onRunEnd();

    QObject::connect(startBtn, &QPushButton::clicked, [this](){
        this->onRunStart();
        this->runService.start([this](){this->onRunEnd();});
    });

    QObject::connect(stopBtn, &QPushButton::clicked, [this](){
        this->runService.stop();
    });

    QObject::connect(timer, &QTimeEdit::userTimeChanged, [this](){
        int t = timer->time().hour();
        t = t*60 + timer->time().minute();
        t = t*60 + timer->time().second();

        this->runService.setTimer(t);
    });

    QPushButton* mainComuterConfigureBtn = new QPushButton("configure\nmain computer", m_toolBar);
    m_toolBar->addWidget(mainComuterConfigureBtn);

    QObject::connect(mainComuterConfigureBtn, &QPushButton::clicked, [this](){
        gui.show();
    });
}


void MainWindow::onRunStart()
{
    startBtn->hide();
    timer->hide();

    stopBtn->show();
    m_toolbar_blocker->show();
    m_scene_blocker->show();
    stopBtn->raise();
}

void MainWindow::onRunEnd()
{
    startBtn->show();
    timer->show();

    stopBtn->hide();
    m_toolbar_blocker->hide();
    m_scene_blocker->hide();
}

void MainWindow::background_Layout() {
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Image Files (*.png *.jpg *.bmp)");
    if (!imagePath.isEmpty()) {
        QImage backgroundImage(imagePath);
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(backgroundImage));
        pixmapItem->setPos(0, 0);
        m_scene->addItem(pixmapItem);
    }
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
