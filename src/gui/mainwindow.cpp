#include "mainwindow.h"
#include <QGraphicsView>
#include <QToolBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QWidget>
#include <QPushButton>
#include <QRandomGenerator>
#include "SimulationControlPanel.h"
#include <QRect>
#include <QHBoxLayout>

#include "customwidget.h"
#include "client/websocketclient.h"
#include "client/RunHandler.h"

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent), 
        m_scene(new CustomScene()),
        m_runService(std::make_shared<RunService>()) {
    m_view = new QGraphicsView(m_scene);

    setupToolBar();

    auto mainWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();
    m_topLayout->addWidget(m_toolBar);
    m_topLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_topLayout);

    // Create a frame to wrap the main layout
    mainFrame = new QFrame();
    mainFrame->setFrameShape(QFrame::Box);
    mainFrame->setLineWidth(2);
    mainFrame->setStyleSheet("QFrame { border: 2px solid black; }");
    mainFrame->setLayout(m_mainLayout);

    // Set the frame as the central widget
    auto centralWidgetLayout = new QVBoxLayout(mainWidget);
    centralWidgetLayout->addWidget(mainFrame);
    mainWidget->setLayout(centralWidgetLayout);
    setCentralWidget(mainWidget);

    // Create a new toolbar for the right side
    rightToolBar = new QToolBar("popup", this);
    rightToolBar->setFixedWidth(150);
    addToolBar(Qt::RightToolBarArea, rightToolBar);
    m_scene->rightToolBar = rightToolBar;
    m_popupDialog = new PopupDialog(rightToolBar);
    m_scene->popupDialog = m_popupDialog;
    qInfo() << "set popupDialog";



    auto toolBar = addToolBar("Tools");
    toolBar->addAction("Background", [this] { background_Layout(); });
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

    // Initialize the connection status label
    m_connectionStatusLabel = new QLabel("Disconnected", this);
    m_connectionStatusLabel->setStyleSheet("QLabel { color : red; }");
    m_connectionStatusLabel->setAlignment(Qt::AlignCenter);

    // Create a layout for the connection status
    QVBoxLayout* connectionStatusLayout = new QVBoxLayout();
    connectionStatusLayout->addWidget(m_connectionStatusLabel);

    // Add the connection status label to the toolbar
    m_toolBar->addWidget(m_connectionStatusLabel);

    addToolBar(Qt::LeftToolBarArea, m_toolBar);
}

void MainWindow::setupRunService()
{
    m_runService->setScene(m_scene);

    startBtn = new QPushButton("start", m_toolBar);
    // m_toolBar->addWidget(startBtn);
    stopBtn = new QPushButton("stop", m_toolBar);
    // m_toolBar->addWidget(stopBtn);
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
    m_DB_handler = new DB_handler();
    qInfo () << " new handler";

    onRunEnd();
    QObject::connect(startBtn, &QPushButton::clicked, [this] {
        this->onRunStart();
        this->m_runService->start([this] { this->onRunEnd(); });
        WebSocketClient::getInstance().sendMessage(QJsonObject{
            {"action", "run"},
            {"command", "start"}
        });
    });

    QObject::connect(stopBtn, &QPushButton::clicked, [this] {
        this->m_runService->stop();
        WebSocketClient::getInstance().sendMessage(QJsonObject{
            {"action", "run"},
            {"command", "stop"}
        });
    });

    QObject::connect(timer, &QTimeEdit::userTimeChanged, [this] {
        int t = timer->time().hour();
        t = t * 60 + timer->time().minute();
        t = t * 60 + timer->time().second();

        this->m_runService->setTimer(t);
    });

    WebSocketClient::getInstance().addActionHandler("run", std::make_unique<RunHandler>(
        [this] { 
            this->onRunStart();
            this->m_runService->start([this] { this->onRunEnd(); });
         },
        [this] { this->m_runService->stop(); }
    ));
}

void MainWindow::onConnectionStatusChanged(bool connected) {
    if (connected) {
        m_connectionStatusLabel->setText("Connected");
        m_connectionStatusLabel->setStyleSheet("QLabel { color : green; }");
        mainFrame->setStyleSheet("QFrame { border: 2px solid green; }");
    } else {
        m_connectionStatusLabel->setText("Disconnected");
        m_connectionStatusLabel->setStyleSheet("QLabel { color : red; }");
        mainFrame->setStyleSheet("QFrame { border: 2px solid red; }");
    }
}
void MainWindow::fill_box_data() {
    QMap<wint_t, QList<QString>> json_names;
    for (const QJsonValue &value: itemsArray) {
        QJsonObject obj = value.toObject();
        wint_t id = obj["id"].toInt();
        QJsonArray dataArray = obj["data"].toArray();

        QList<QString> namesList;
        for (const QJsonValue &dataValue: dataArray) {
            QJsonObject dataObj = dataValue.toObject();
            namesList << dataObj["name"].toString();
        }
        json_names[id] = namesList;
    }
    for (auto item: m_scene->items()) {
        if (BaseItem *base = dynamic_cast<SensorItem *>(item)) {
            auto *sensor = dynamic_cast<SensorItem *>(item);
            QString sensorId = sensor->getPriority();
            int intValue = sensorId.toInt();
            auto wideIntValue = static_cast<wint_t>(intValue);
            if (json_names.contains(wideIntValue)) {
                base->names = json_names.value(wideIntValue);
            } else {
                base->names = json_names.value(0);
            }
        }
    }
}
void MainWindow::read_from_json() {
    const QString& filePath = QDir::currentPath() + "/box_info.json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file" << filePath << ": " << file.errorString();
        return;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &parseError);
    if (document.isNull()) {
        qWarning() << "Failed to create JSON doc.";
        qWarning() << "Error:" << parseError.errorString();
        qWarning() << "Offset:" << parseError.offset;
        return;
    }
    if (!document.isObject()) {
        qWarning() << "JSON is not an object.";
        return;
    }
    QJsonObject jsonObj = document.object();
    itemsArray = jsonObj["items"].toArray();
    qInfo() << "Successfully read from JSON. Number of items:" << itemsArray.size();
}

void MainWindow::fill_db_data()
{
    for (const auto &value : itemsArray) {
        QJsonObject obj = value.toObject();
        wint_t id = obj["id"].toInt();
        QJsonArray dataArray = obj["data"].toArray();

        QList<QList<QString>> itemData;
        for (const auto &dataValue : dataArray) {
            QJsonObject dataObj = dataValue.toObject();
            QList<QString> row;
            row << dataObj["name"].toString()
                << dataObj["numBits"].toString()
                << dataObj["type"].toString();
            itemData.append(row);
        }
        m_DB_handler->data_of_sensors[id] = itemData;
    }
    int numKeys = m_DB_handler->data_of_sensors.size();
    qDebug() << "Number of keys (sensors):" << numKeys;

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
    }
}

void MainWindow::replayer() {
    close_previous_replay();
    loadLayout();
    QString logFilePath = QFileDialog::getOpenFileName(this, "Select log file", "", "Log Files (*.log)");
    if (!logFilePath.isEmpty()) {
        m_liveUpdate_forReplyer = std::make_unique<LiveUpdate>(m_scene);
        m_simulationReplayer = std::make_unique<SimulationReplayer>(logFilePath, m_DB_handler, std::move(m_liveUpdate_forReplyer), m_scene, this);
        m_simulationReplayer->startReplay();
        controlPanel = new SimulationControlPanel(m_simulationReplayer.get(), this);
        m_mainLayout->addWidget(controlPanel);
        read_from_json();
        fill_db_data();
        fill_box_data();
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
bson_t* sensor_to_bson_obj(SensorItem* sensor){
    bson_t* base_BSON = bson_new();
    BSON_APPEND_UTF8(base_BSON, "type", "Sensor");
    BSON_APPEND_DOUBLE(base_BSON, "pos_x", sensor->pos().x());
    BSON_APPEND_DOUBLE(base_BSON, "pos_y", sensor->pos().y());
    BSON_APPEND_INT32(base_BSON, "red", sensor->get_m_color().red());
    BSON_APPEND_INT32(base_BSON, "green", sensor->get_m_color().green());
    BSON_APPEND_INT32(base_BSON, "blue", sensor->get_m_color().blue());
    BSON_APPEND_UTF8(base_BSON, "id", sensor->getPriority().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "name", sensor->getName().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "buildCommand", sensor->getBuildCommand().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "runCommand", sensor->getRunCommand().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "cmakePath", sensor->getCmakePath().toUtf8().constData());
    return base_BSON;
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

void MainWindow::onRunStart()
{
    read_from_json();
    fill_db_data();
    fill_box_data();
    // for test only
    m_bufferTest = new buffer_test(); // this generates buffer every 2 seconds, and write then to A.log
    // end text
    const QString& filePath = QDir::currentPath() + "/A.log";
    m_logReader = std::make_unique<LogReader>(filePath, m_DB_handler, nullptr, this);
    if(m_simulationRecorder != nullptr){
        m_logReader->m_simulationRecorder = std::move(m_simulationRecorder);
    }
    change_view_timer = new QTimer(this);
    connect(change_view_timer, &QTimer::timeout, this, &MainWindow::update_view);
    change_view_timer->start(5000);


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

void MainWindow::update_view() {
    QMap<QString, QVariantList> last_changes;
    for (auto item : m_scene->items()) {
        if (auto *sensor = dynamic_cast<SensorItem *>(item)) {
            QString sensorId = sensor->getPriority();
            QList<QVariant> data = m_DB_handler->read_all_from_DB(sensorId);
//            QList<QVariant> data = m_DB_handler->read_last_from_DB(sensorId);
            sensor->update_db_data(data);
            last_changes[sensorId] = data;
        }
    }
    m_liveUpdate_forLogger->parse_new_data(last_changes);
}

void MainWindow::create_sensor_from_bson_obj(const bson_t *bsonDocument) {
    auto *new_sensor = new SensorItem();
    bson_iter_t iter;
    bson_iter_init(&iter, bsonDocument);
    bson_iter_next(&iter);

    // location
    bson_iter_next(&iter);
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

    // sensor info
    bson_iter_next(&iter);
    new_sensor->setPriority(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setName(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setBuildCommand(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setRunCommand(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setCmakePath(bson_iter_utf8(&iter, nullptr));

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
    // add popup to the load sensors
    for (auto item: m_scene->items()) {
        if (auto *sensor = dynamic_cast<SensorItem *>(item)) {
            if(sensor->popupDialog == nullptr){
                sensor->popupDialog = m_popupDialog;
            }
        }
    }
}



