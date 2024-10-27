#include "mainwindow.h"
#include <QThread>
#include <thread>
#include <QPushButton>
#include <QFileDialog>



#include "app_utils.h"

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent), 
        m_scene(new CustomScene()),
        m_runService(std::make_shared<RunService>()),
        m_globalState(GlobalState::getInstance()),
        m_initializeSensorsData(new initializeSensorsData()),
        m_mainWindowTitle("Vehicle sensors simulator"),
        m_countdownTimer(new QTimer(this))
{
    m_saveAndLoad = new saveAndLoad(&m_globalState);
    m_parser = new parser();
    // for test only
    m_bufferTest = new buffer_test();

    setupToolBar();

    setWindowTitle(m_mainWindowTitle);

    auto mainWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout();
    m_centerLayout = new QHBoxLayout();
    m_centerLayout->addWidget(m_toolBar);
    m_mainLayout->addLayout(m_centerLayout);

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
    rightToolBar = EditPanel::getPanel();
    rightToolBar->setParent(this);
    rightToolBar->setFixedWidth(200);
    addToolBar(Qt::RightToolBarArea, rightToolBar);


    auto toolBar = addToolBar("Tools");
    toolBar->addAction("Background", [this] { background_Layout(); });
    toolBar->addAction( "Save", [this] { saveLayout(); });
    toolBar->addAction("Load", [this] { loadLayout(); });
    toolBar->addAction("Record", [this] { record(); });
    toolBar->addAction("Replay", [this] { replayer(); });
    toolBar->addAction("mainComp", [this] { mainComputer.openEditor(); });

    setupView();

    // Connect to GlobalState
    connect(&m_globalState, &GlobalState::connectionStateChanged, this, &MainWindow::onConnectionStatusChanged);
    connect(&m_globalState, &GlobalState::currentProjectChanged, this, &MainWindow::onCurrentProjectChanged);
    connect(&m_globalState, &GlobalState::currentProjectPublished, this, &MainWindow::onCurrentProjectPublished);

    connect(m_countdownTimer, &QTimer::timeout, this, &MainWindow::updateTimer);

    // for test only
    connect(&m_globalState, &GlobalState::new_test_buffer_arrived, this, &MainWindow::buffer_listener);


    ProjectModel* startingProject = new ProjectModel("Project");
    m_globalState.addProject(startingProject);
    m_globalState.setCurrentProject(startingProject);

    tabWidget = new QTabWidget();
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    createRemoteTab();
    createTerminalTab();

    m_mainLayout->addWidget(tabWidget);

    connect(&m_globalState, &GlobalState::currentProjectChanged, this, &MainWindow::onCurrentProjectChanged);
    connect(&m_globalState, &GlobalState::newLogArrived, this, &MainWindow::handleNewLog);
    connect(&m_globalState, &GlobalState::tabPressed, this, &MainWindow::pressOnTab);

    setupRunService();
}
void MainWindow::createRemoteTab() {
    QWidget* remoteTab = new QWidget();
    QVBoxLayout* remoteTabLayout = new QVBoxLayout(remoteTab);
    remoteTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    remoteTab->setMinimumSize(400, 120);

    m_remoteInterface = new RemoteInterface(this);
    remoteTabLayout->addWidget(m_remoteInterface);

    int index = tabWidget->addTab(remoteTab, "Remote");

    TabInfo info{remoteTab, nullptr, index, "Remote"};
    tabInfoMap["Remote"] = info;
}

void MainWindow::createTerminalTab() {
    QWidget* terminalTab = new QWidget();
    QVBoxLayout* terminalTabLayout = new QVBoxLayout(terminalTab);
    QTextEdit* logTextEdit = new QTextEdit();
    terminalTabLayout->addWidget(logTextEdit);

    int index = tabWidget->addTab(terminalTab, "Terminal");

    TabInfo info{terminalTab, logTextEdit, index, "Terminal"};
    tabInfoMap["Terminal"] = info;
}

void MainWindow::handleProjectConnections(ProjectModel* newProject) {
    if (m_currentProject) {
        disconnect(m_currentProject, &ProjectModel::modelAdded, this, &MainWindow::createNewTab);
        disconnect(m_currentProject, &ProjectModel::modelRemoved, this, &MainWindow::removeTab);
        disconnect(m_currentProject, &ProjectModel::modelUpdated, this, &MainWindow::updateTab);
    }

    connect(newProject, &ProjectModel::modelAdded, this, &MainWindow::createNewTab);
    connect(newProject, &ProjectModel::modelRemoved, this, &MainWindow::removeTab);
    connect(newProject, &ProjectModel::modelUpdated, this, &MainWindow::updateTab);
}

void MainWindow::createNewTab(SerializableItem* model) {
    if (auto* pSensorModel = dynamic_cast<SensorModel*>(model)) {
        QString tabName;
        if(auto* pQemuModel = dynamic_cast<QemuSensorModel*>(model)){
            tabName = "Qemu " + pQemuModel->priority();
        }else{
            tabName = "Sensor " + pSensorModel->priority();
        }
        auto* newTab = new QWidget();
        auto* tabLayout = new QVBoxLayout(newTab);
        auto* textEdit = new QTextEdit();
        tabLayout->addWidget(textEdit);
        int index = tabWidget->addTab(newTab, tabName);

        TabInfo info{newTab, textEdit, index, pSensorModel->getId()};
        tabInfoMap[tabName] = info;
    }
}

void MainWindow::updateTab(SerializableItem* model) {
    if (auto* pSensorModel = dynamic_cast<SensorModel*>(model)) {
        QString newTabName;
        if(auto* pQemuModel = dynamic_cast<QemuSensorModel*>(model)){
            newTabName = "Qemu " + pQemuModel->priority();
        }else{
            newTabName = "Sensor " + pSensorModel->priority();
        }
        auto it = std::find_if(tabInfoMap.begin(), tabInfoMap.end(),
                               [&](const auto& pair) { return pair.second.modelId == pSensorModel->getId(); });

        if (it != tabInfoMap.end()) {
            QString oldTabName = it->first;
            TabInfo info = it->second;
            tabWidget->setTabText(info.index, newTabName);
            tabInfoMap.erase(it);
            tabInfoMap[newTabName] = info;
        }
    }
}

void MainWindow::removeTab(SerializableItem* model) {
    if (auto* pSensorModel = dynamic_cast<SensorModel*>(model)) {
        auto it = std::find_if(tabInfoMap.begin(), tabInfoMap.end(),
                               [&](const auto& pair) { return pair.second.modelId == pSensorModel->getId(); });

        if (it != tabInfoMap.end()) {
            tabWidget->removeTab(it->second.index);
            delete it->second.widget;
            tabInfoMap.erase(it);
        }
    }
}

void MainWindow::removeAllTabs() {
    for (auto it = tabInfoMap.begin(); it != tabInfoMap.end();) {
        if (it->first != "Terminal" && it->first != "Remote") {
            tabWidget->removeTab(it->second.index);
            delete it->second.widget;
            it = tabInfoMap.erase(it);
        } else {
            ++it;
        }
    }
}
void MainWindow::pressOnTab(const QString& tabName) {
    QString qemuName = "Qemu " + tabName;
    QString sensorName = "Sensor " + tabName;
    auto it = tabInfoMap.find(tabName);
    if (it == tabInfoMap.end()) {
        it = tabInfoMap.find(qemuName);
    }
    if (it == tabInfoMap.end()) {
        it = tabInfoMap.find(sensorName);
    }
    if (it != tabInfoMap.end()) {
        tabWidget->setCurrentIndex(it->second.index);
    } else {
        qWarning() << "Error in pressOnTab: Tab not found. Tab name:" << tabName;
    }
}

void MainWindow::handleNewLog(const QString& newLog, const QString& tabName) {
    QString qemuName = "Qemu " + tabName;
    QString sensorName = "Sensor " + tabName;
    auto it = tabInfoMap.find(tabName);
    if (it == tabInfoMap.end()) {
        it = tabInfoMap.find(qemuName);
    }
    if (it == tabInfoMap.end()) {
        it = tabInfoMap.find(sensorName);
    }

    if (it != tabInfoMap.end() && it->second.textEdit) {
        it->second.textEdit->append(newLog);
    } else {
        qWarning() << "Error in handleNewLog: Tab or QTextEdit not found. Tab name:" << tabName;
    }
}

void MainWindow::setupToolBar() {
    m_toolBar = new QToolBar("Shapes", this);
    m_toolBar->setOrientation(Qt::Vertical);

    auto rItemWidget = new CustomWidget(CustomWidget::REGULAR_SENSOR_ITEM, this);
    m_toolBar->addWidget(rItemWidget);

    auto qemuItemWidget = new CustomWidget(CustomWidget::QEMU_SENSOR_ITEM, this);
    m_toolBar->addWidget(qemuItemWidget);

    // auto busWidget = new CustomWidget(CustomWidget::BUS_ITEM, this);
    // m_toolBar->addWidget(busWidget);

    addToolBar(Qt::LeftToolBarArea, m_toolBar);
}

void MainWindow::setupRunService()
{
    QObject::connect(m_runService.get(), &RunService::newCommunicationPacketAccepted, [](QString packet){
        qInfo() << "------------------------------packet-----------------------------";
        GlobalState::getInstance().log("------------------------------packet-----------------------------",
                                       "Terminal");
        qInfo() << packet;
        GlobalState::getInstance().log(packet, "Terminal");
        qInfo() << "------------------------------packet-----------------------------";
        GlobalState::getInstance().log("------------------------------packet-----------------------------",
                                       "Terminal");
    });

    QObject::connect(m_runService.get(), &RunService::newCommunicationPacketAccepted, &GlobalState::getInstance(), &GlobalState::newData);

    m_startBtn = new IconButton(globalConstants::Icons::Start, "Start", m_toolBar);
    m_stopBtn = new IconButton(globalConstants::Icons::Stop, "Stop", m_toolBar);
    
    m_timer = new QTimeEdit(m_toolBar);
    m_timer->setDisplayFormat("hh:mm:ss");
    m_timer->setFixedSize(120, 30);
    m_timer->setCurrentSection(QDateTimeEdit::MinuteSection);

    m_buttonStack = new QStackedWidget(m_toolBar);
    m_buttonStack->addWidget(m_startBtn);
    m_buttonStack->addWidget(m_stopBtn);
    m_buttonStack->setCurrentWidget(m_startBtn);
    m_buttonStack->setStyleSheet("QStackedWidget { border: none; background: none; }");
    m_buttonStack->setFixedSize(50, 50);

    m_toolBar->addWidget(m_buttonStack);
    m_toolBar->addWidget(m_timer);

    m_toolbar_blocker = new ActionsBlocker(m_toolBar);
    m_scene_blocker = new ActionsBlocker(m_view);
    m_scene_blocker->transparency(0.0);

    onRunEnd();
    QObject::connect(m_runService.get(), &RunService::stopFinished, [this](){
        onRunEnd();
    });
    QObject::connect(m_runService.get(), &RunService::startBegin, [this](){

    });
    QObject::connect(m_runService.get(), &RunService::newCommunicationPacketAccepted, [this](QString packet){
        qInfo() << packet;
        GlobalState::getInstance().log(packet, "Terminal");

    });
    QObject::connect(m_startBtn, &QPushButton::clicked, [this] {
        WebSocketClient::getInstance().sendMessage(QJsonObject{
            {"action", "run"},
            {"command", "start"},
            {"timer", m_timer->time().toString("hh:mm:ss")},
            {"com_server_ip", QString::fromStdString(App_Utils::getPublicIp()) }
        });
        onRunStart();
    });

    QObject::connect(m_stopBtn, &QPushButton::clicked, [this] {
        // this->m_runService->stop();
        //m_runService->stop();
        WebSocketClient::getInstance().sendMessage(QJsonObject{
            {"action", "run"},
            {"command", "stop"}
        });
        m_runService->stop();
    });



    WebSocketClient::getInstance().addActionHandler("run", std::make_unique<RunHandler>(
        [this](const QString& timer, const QString& com_server_ip) {
            m_timer->setTime(QTime::fromString(timer, "hh:mm:ss"));
            this->onRunStart(com_server_ip);
            //this->m_runService->start(/*[this] { this->onRunEnd(); }*/);
         },
        [this] { this->m_runService->stop(); }
    ));
}

void MainWindow::setupView() {
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_sceneBox = new QGroupBox(this);

    // Initialize the title and conditional button
    m_publishButton = new IconButton(globalConstants::Icons::Publish, "Publish the project", m_sceneBox);
    m_publishButton->setDisabled(m_globalState.connectionState() != globalConstants::ConnectionState::Online);
    QObject::connect(m_publishButton, &QPushButton::clicked, [this] {
        m_globalState.publishCurrentProject();
    });

    // Add the title and button to the layout
    auto sceneBoxLayout = new QVBoxLayout();
    sceneBoxLayout->addWidget(m_publishButton);
    sceneBoxLayout->setAlignment(m_publishButton, Qt::AlignCenter);
    sceneBoxLayout->addWidget(m_view);

    // Set the border style for the layout to none
    m_layoutWidget = new QWidget();
    m_layoutWidget->setLayout(sceneBoxLayout);
    m_layoutWidget->setStyleSheet("QWidget { border: none; }");

    m_sceneBox->setLayout(new QVBoxLayout());
    m_sceneBox->layout()->addWidget(m_layoutWidget);
    m_centerLayout->addWidget(m_sceneBox);
}

void MainWindow::onCurrentProjectChanged(ProjectModel* project) {
    m_sceneBox->setTitle(project->name());
    m_publishButton->setVisible(!project->isPublished());
    handleProjectConnections(project);
    removeAllTabs();
    m_currentProject = project;
    if (m_currentProject) {
        for (SerializableItem* model : m_currentProject->models()) {
            createNewTab(model);
        }
    }

}

void MainWindow::onCurrentProjectPublished(ProjectModel* project) {
    m_publishButton->hide();
}

void MainWindow::onConnectionStatusChanged(globalConstants::ConnectionState state) {
    switch (state){
        case globalConstants::ConnectionState::Online:
            setWindowTitle(m_mainWindowTitle + " - Online");
            mainFrame->setStyleSheet("QFrame { border: 2px solid green; }");
            break;
        case globalConstants::ConnectionState::Offline:
            setWindowTitle(m_mainWindowTitle);
            mainFrame->setStyleSheet("QFrame { border: 2px solid red; }");
            break;
        case globalConstants::ConnectionState::Connecting:
            setWindowTitle(m_mainWindowTitle + " - Connecting...");
            mainFrame->setStyleSheet("QFrame { border: 2px solid orange; }");
            break;
    }
    m_publishButton->setDisabled(state != globalConstants::ConnectionState::Online);
}


void MainWindow::background_Layout() {
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Image Files (*.png *.jpg *.bmp)");
    m_currentMainBackgroundPath = imagePath;
    updateBackground();
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateBackground();
}

void MainWindow::updateBackground()
{
    if (!m_currentMainBackgroundPath.isEmpty()) {
        QImage backgroundImage(m_currentMainBackgroundPath);
        QSize viewSize = m_view->viewport()->size();
        QImage scaledImage = backgroundImage.scaled(viewSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        if (scaledImage.width() > viewSize.width() || scaledImage.height() > viewSize.height()) {
            scaledImage = scaledImage.copy((scaledImage.width() - viewSize.width()) / 2,
                                           (scaledImage.height() - viewSize.height()) / 2,
                                           viewSize.width(),
                                           viewSize.height());
        }
        QBrush imageBrush(scaledImage);
        imageBrush.setTransform(QTransform().translate(-viewSize.width() / 2, -viewSize.height() / 2));
        m_view->setBackgroundBrush(imageBrush);
    }
}

void MainWindow::record() {
    QString selectedDir = QFileDialog::getExistingDirectory(this,
                                                            "Select Directory for Recording Files",
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly |
                                                            QFileDialog::DontResolveSymlinks);
    if (selectedDir.isEmpty()) {
        return;
    }
    QString logFilePath = selectedDir + "/record.log";
    QString bsonFilePath = selectedDir + "/save.bson";
    m_DB_handler = new DB_handler();
    m_simulationRecorder = new SimulationRecorder(logFilePath, m_DB_handler->dbFilePath);
    saveLayout(bsonFilePath);
}
void MainWindow::replayer() {
    close_previous_replay();
    QString selectedDir = QFileDialog::getExistingDirectory(this,
                                                            "Select Directory Containing Replay Files",
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly |
                                                            QFileDialog::DontResolveSymlinks);
    if (selectedDir.isEmpty()) {
        return;
    }
    QDir dir(selectedDir);
    QString bsonFilePath = dir.entryList(QStringList() << "*.bson", QDir::Files).first();
    QString logFilePath = dir.entryList(QStringList() << "*.log", QDir::Files).first();
    if (bsonFilePath.isEmpty() || logFilePath.isEmpty()) {
        QMessageBox::warning(this, "Missing Files",
                             "The selected directory must contain both a .bson and a .log file.");
        return;
    }
    bsonFilePath = dir.filePath(bsonFilePath);
    logFilePath = dir.filePath(logFilePath);
    loadLayout(bsonFilePath);
    m_simulationReplayer = new SimulationReplayer(logFilePath);
    controlPanel = new SimulationControlPanel(m_simulationReplayer, this);
    connect(&m_globalState, &GlobalState::delAllTabContent, this, &MainWindow::resetTabContent);
    m_mainLayout->addWidget(controlPanel);
    m_initializeSensorsData->initialize();
}
void MainWindow::resetTabContent() {
    for (auto& pair : tabInfoMap) {
        if (pair.second.textEdit) {
            pair.second.textEdit->clear();
        }
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

void MainWindow::onRunStart(QString com_server_ip)
{
    int t = m_timer->time().hour();
    t = t * 60 + m_timer->time().minute();
    t = t * 60 + m_timer->time().second();

    m_runService->start(t, com_server_ip);

    m_initializeSensorsData->initialize();
    close_previous_replay();
    resetTabContent();
    if(m_DB_handler == nullptr){
        m_DB_handler = new DB_handler();
    }
    // for test only
    m_bufferTest->start_timer();

    m_globalState.setIsRunning(true);
    // m_startBtn->hide();
    // m_timer->hide();
    // m_stopBtn->show();
    // //m_toolbar_blocker->show();
    // //m_scene_blocker->show();
    // m_stopBtn->raise();
    m_buttonStack->setCurrentWidget(m_stopBtn);
    m_timer->setDisabled(true);

    m_toolBar->update();
    m_toolBar->repaint();

    m_countdownTimer->start(1000);
}

void MainWindow::onRunEnd()
{
    m_buttonStack->setCurrentWidget(m_startBtn);
    m_timer->setDisabled(false);

    m_globalState.setIsRunning(false);
    // m_stopBtn->hide();
    m_toolbar_blocker->hide();
    m_scene_blocker->hide();
    m_countdownTimer->stop();

    // for test only
    m_bufferTest->stop_timer();

}

void MainWindow:: buffer_listener(const QString &data) {
    m_globalState.newData(data);
}

void MainWindow::saveLayout(const QString &dirPath) {
    m_globalState.saveData(dirPath);
}
void MainWindow::loadLayout(const QString &dirPath) {
    close_previous_replay();
    m_globalState.loadData(dirPath);
}

void MainWindow::updateTimer()
{
    QTime currentTime = m_timer->time();
    if (currentTime == QTime(0, 0, 0)) {
        m_countdownTimer->stop();
        // m_runService->stop();
        return;
    }
    currentTime = currentTime.addSecs(-1);
    m_timer->setTime(currentTime);
}
