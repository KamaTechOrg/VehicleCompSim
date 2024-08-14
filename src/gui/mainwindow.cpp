#include "mainwindow.h"
#include "customwidget.h"
#include <QGraphicsView>
#include <QToolBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include "client/websocketclient.h"
#include <QGraphicsItem>
#include <QWidget>
#include <QPushButton>
#include <QRandomGenerator>
#include "SimulationControlPanel.h"
#include <QRect>
#include <bson/bson.h>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent), m_scene(new CustomScene()) {
    m_view = new QGraphicsView(m_scene);

    setupToolBar();

    auto mainWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(mainWidget);
    m_topLayout = new QHBoxLayout();
    m_topLayout->addWidget(m_toolBar);
    m_topLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_topLayout);
    setCentralWidget(mainWidget);

    auto toolBar = addToolBar("Tools");
    toolBar->addAction( "Background", [this] { background_Layout(); });
    toolBar->addAction( "Save", [this] { saveLayout(); });
    toolBar->addAction("Load", [this] { loadLayout(); });
    toolBar->addAction("Record", [this] { record(); });
    toolBar->addAction("Replay", [this] { replayer(); });

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

void MainWindow::setupRunService() {
    runService.setScene(m_scene);

    startBtn = new QPushButton("Start", m_toolBar);
    stopBtn = new QPushButton("Stop", m_toolBar);
    timer = new QTimeEdit(m_toolBar);
    timer->setDisplayFormat("hh:mm:ss");
    timer->setFixedSize(120, 30);
    timer->setCurrentSection(QDateTimeEdit::MinuteSection);

    m_toolBar->addWidget(startBtn);
    m_toolBar->addWidget(stopBtn);
    m_toolBar->addWidget(timer);

    m_toolbar_blocker = new ActionsBlocker(m_toolBar);
    m_scene_blocker = new ActionsBlocker(m_view);
    m_scene_blocker->transparency(0.0);
    m_liveUpdate_forLogger = std::make_unique<LiveUpdate>(m_scene);
    m_logReader = std::make_unique<LogReader>(R"(C:\Users\OWNER\Downloads\A.log)", std::move(m_liveUpdate_forLogger), nullptr, this);

    onRunEnd();
    QObject::connect(startBtn, &QPushButton::clicked, [this] {
        this->onRunStart();
        this->runService.start([this] { this->onRunEnd(); });
    });

    QObject::connect(stopBtn, &QPushButton::clicked, [this] {
        this->runService.stop();
    });

    QObject::connect(timer, &QTimeEdit::userTimeChanged, [this] {
        int t = timer->time().hour();
        t = t * 60 + timer->time().minute();
        t = t * 60 + timer->time().second();

        this->runService.setTimer(t);
    });
}

void MainWindow::onConnectionStatusChanged(bool connected)
{
  
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
void MainWindow::record() {
    QString defaultFileName = "record.log";
    QString logFilePath = QFileDialog::getSaveFileName(nullptr, "Select or create log file", defaultFileName, "Log Files (*.log)");
    if (!logFilePath.isEmpty()) {
        m_simulationRecorder = std::make_unique<SimulationRecorder>(logFilePath);
        m_logReader->m_simulationRecorder = std::move(m_simulationRecorder);
    }
    saveLayout();
}

void MainWindow::replayer() {
    close_previous_replay();
    loadLayout();
    QString logFilePath = QFileDialog::getOpenFileName(this, "Select log file", "", "Log Files (*.log)");
    if (!logFilePath.isEmpty()) {
        m_liveUpdate_forReplyer = std::make_unique<LiveUpdate>(m_scene);
        m_simulationReplayer = std::make_unique<SimulationReplayer>(logFilePath, std::move(m_liveUpdate_forReplyer), this);
        m_simulationReplayer->startReplay();
        controlPanel = new SimulationControlPanel(m_simulationReplayer.get(), this);
        m_mainLayout->addWidget(controlPanel);
    }
}
void MainWindow::close_previous_replay(){
    if (controlPanel != nullptr) {
        m_simulationReplayer->clear_current_events();
        controlPanel->close();
        delete controlPanel;
        controlPanel = nullptr;
    }
}


bson_t* sensor_to_bson_obj(SensorItem* sensor){
    bson_t* base_BSON = bson_new();
    BSON_APPEND_UTF8(base_BSON, "type", "Sensor");
    BSON_APPEND_DOUBLE(base_BSON, "unique_id", sensor->get_unique_id());
    BSON_APPEND_DOUBLE(base_BSON, "pos_x", sensor->pos().x());
    BSON_APPEND_DOUBLE(base_BSON, "pos_y", sensor->pos().y());
    BSON_APPEND_INT32(base_BSON, "red", sensor->get_m_color().red());
    BSON_APPEND_INT32(base_BSON, "green", sensor->get_m_color().green());
    BSON_APPEND_INT32(base_BSON, "blue", sensor->get_m_color().blue());
//    BSON_APPEND_UTF8(base_BSON, "id", sensor->getID().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "name", sensor->getName().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "buildCommand", sensor->getBuildCommand().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "runCommand", sensor->getRunCommand().toUtf8().constData());
    return base_BSON;
}

void SaveBsonToFile(std::vector<bson_t*> &bson_obj_vector) {
    QString defaultFileName = "layout.bson";
    QString selectedFileName = QFileDialog::getSaveFileName(nullptr, "Save BSON File", defaultFileName, "BSON Files (*.bson);;All Files (*)");
    if (!selectedFileName.isEmpty()) {
        QFile outputFile(selectedFileName);
        if (outputFile.open(QIODevice::WriteOnly)) {
            for(bson_t * bson_obj : bson_obj_vector) {
                const uint8_t *bsonData = bson_get_data(bson_obj);
                outputFile.write(reinterpret_cast<const char *>(bsonData), bson_obj->len);
            }
            outputFile.close();
        }
    }
}

void MainWindow::saveLayout() {
    std::vector<bson_t*> base_BSON_vector;
    for (auto item : m_scene->items()) {
        if (BaseItem* base = dynamic_cast<SensorItem*>(item)) {
            base_BSON_vector.push_back(sensor_to_bson_obj(dynamic_cast<SensorItem*>(item)));
        }
    }
    SaveBsonToFile(base_BSON_vector);
}
void MainWindow::create_sensor_from_bson_obj(const bson_t *bsonDocument) {
    auto *new_sensor = new SensorItem();
    bson_iter_t iter;
    bson_iter_init(&iter, bsonDocument);
    bson_iter_next(&iter);
    bson_iter_next(&iter);

    new_sensor->set_unique_id(bson_iter_double(&iter));
    bson_iter_next(&iter);

    // location
    double x = bson_iter_double(&iter);
    bson_iter_next(&iter);
    double y = bson_iter_double(&iter);
    new_sensor->setPos(x, y);

    // color
    bson_iter_next(&iter);
    int r = bson_iter_int32(&iter);
    bson_iter_next(&iter);
    int g = bson_iter_int32(&iter);
    bson_iter_next(&iter);
    int b = bson_iter_int32(&iter);
    new_sensor->set_m_color(r,g,b);
//    bson_iter_next(&iter);

    // sensor info
//    new_sensor->setID(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setName(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setBuildCommand(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setRunCommand(bson_iter_utf8(&iter, nullptr));

    m_scene->addItem(new_sensor);
    new_sensor->update();
}

void MainWindow::loadLayout() {
    m_scene->clear();
    QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Select BSON File"), QString(),
                                                            tr("BSON Files (*.bson);;All Files (*)"));
    if (!selectedFileName.isEmpty()) {
        bson_reader_t* reader = bson_reader_new_from_file(selectedFileName.toUtf8().constData(), NULL);
        const bson_t* bsonDocument;
        while ((bsonDocument = bson_reader_read(reader, NULL))) {
            create_sensor_from_bson_obj(bsonDocument);
        }
        bson_reader_destroy(reader);
    } else {
        qInfo() << "File selection canceled by the user.";
    }
}


