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
    m_DB_handler = new DB_handler();
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

    // Create a QTabWidget
    tabWidget = new QTabWidget();
    // remote tab
    QWidget* remoteTab = new QWidget();
    QVBoxLayout* remoteTabLayout = new QVBoxLayout(remoteTab);
    remoteTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    remoteTab->setMinimumSize(400, 80);
    m_remoteInterface = new RemoteInterface(this);
    remoteTabLayout->addWidget(m_remoteInterface);
    remoteTab->setLayout(remoteTabLayout);
    tabWidget->addTab(remoteTab, "Remote");
    tabIndexMap["Remote"] = tabWidget->indexOf(remoteTab);
    // terminal tab
    QWidget* terminalTab = new QWidget();
    QVBoxLayout* tab2Layout = new QVBoxLayout(terminalTab);
    QTextEdit *logTextEdit = new QTextEdit();
    tab2Layout->addWidget(logTextEdit);
    tabWidget->addTab(terminalTab, "Terminal");
    textEditMap["Terminal"] = logTextEdit;
    tabIndexMap["Terminal"] = tabWidget->indexOf(terminalTab);

    m_mainLayout->addWidget(tabWidget);
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    connect(&m_globalState, &GlobalState::newLogArrived, this, &MainWindow::handleNewLog);
    connect(&m_globalState, &GlobalState::newTab, this, &MainWindow::createNewTab);
    connect(&m_globalState, &GlobalState::tabPressed, this, &MainWindow::pressONTab);

    setupRunService();
}
void MainWindow::createNewTab(const QString &tabName, const QString &oldTabName){
    auto it = tabIndexMap.find(oldTabName);
    if (it != tabIndexMap.end()) {
        int oldTabIndex = tabIndexMap[oldTabName];
        tabWidget->setTabText(oldTabIndex, tabName);
        textEditMap[tabName] = textEditMap[oldTabName];
        tabIndexMap[tabName] = oldTabIndex;
        textEditMap.erase(oldTabName);
    } else {
        auto* newTab = new QWidget();
        auto* tabLayout = new QVBoxLayout(newTab);
        auto *TextEdit = new QTextEdit();
        tabLayout->addWidget(TextEdit);
        tabWidget->addTab(newTab, tabName);
        textEditMap[tabName] = TextEdit;
        tabIndexMap[tabName] = tabWidget->indexOf(newTab);;
    }
}
void MainWindow::pressONTab(const QString & tabName){
    tabWidget->setCurrentIndex(tabIndexMap[tabName]);
}

void MainWindow::handleNewLog(const QString &newLog, const QString &tabName) {
    try {
        auto it = textEditMap.find(tabName);
        if (it == textEditMap.end()) {
            throw std::out_of_range("Tab name not found in textEditMap");
        }
        it->second->append(newLog);
    } catch (const std::out_of_range& e) {
        qWarning() << "Error in handleNewLog: " << e.what() << ". Tab name: " << tabName;
        QString msg = "Error in handleNewLog: " + QString(e.what()) + ". Tab name: " + tabName;
        GlobalState::getInstance().log(msg ,"Terminal");
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
//    QString imagePath1 = QFileDialog::getOpenFileName(this, "Select Image", "", "Image Files (*.png *.jpg *.bmp)");
//    m_currentFrameBackgroundPath = imagePath1;
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
    QString defaultFileName = "record.log";
    QString logFilePath = QFileDialog::getSaveFileName(nullptr, "Select or create log file", defaultFileName, "Log Files (*.log)");
    if (!logFilePath.isEmpty()) {
        m_simulationRecorder = new SimulationRecorder(logFilePath, m_DB_handler->dbFilePath);
    }
}

void MainWindow::replayer() {
    close_previous_replay();
    loadLayout();
    QString logFilePath = QFileDialog::getOpenFileName(this, "Select log file", "", "Log Files (*.log)");
    if (!logFilePath.isEmpty()) {
        m_simulationReplayer = new SimulationReplayer(logFilePath);
        controlPanel = new SimulationControlPanel(m_simulationReplayer, this);
        connect(&m_globalState, &GlobalState::delAllTabContent, this, &MainWindow::resetTabContent);
        m_mainLayout->addWidget(controlPanel);
        m_initializeSensorsData->initialize();
    }
}
void MainWindow::resetTabContent(){
    for (auto& item : textEditMap) {
        item.second->clear(); // Clear the content
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

    resetTabContent();

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

// for test only
void MainWindow:: buffer_listener(const QString &data) {
    m_globalState.newData(data);
}

void MainWindow::saveLayout() {
    m_globalState.saveData();
}
void MainWindow::loadLayout() {
    m_globalState.loadData();
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
