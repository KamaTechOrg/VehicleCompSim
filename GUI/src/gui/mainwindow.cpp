#include "mainwindow.h"
#include "../../MainComputer/src/maincomputer.h"
#include <thread>
#include "app_utils.h"
///#include "../Communication/User_Directory/client/client.h"

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent), 
        m_scene(new CustomScene()),
        m_runService(std::make_shared<RunService>()),
        m_globalState(GlobalState::getInstance()),
        m_initializeSensorsData(new initializeSensorsData()),
        m_mainWindowTitle("Vhiecal sensors simulator")
{

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
    rightToolBar->setFixedWidth(250);
    addToolBar(Qt::RightToolBarArea, rightToolBar);


    auto toolBar = addToolBar("Tools");
    toolBar->addAction("Background", [this] { background_Layout(); });
    toolBar->addAction( "Save", [this] { saveLayout(); });
    toolBar->addAction("Load", [this] { loadLayout(); });
    toolBar->addAction("Record", [this] { record(); });
    toolBar->addAction("Replay", [this] { replayer(); });
    toolBar->addAction("mainComp", [this] { MainComputer().openEditor(); });

    setupView();

    // Connect to GlobalState
    connect(&m_globalState, &GlobalState::connectionStateChanged, this, &MainWindow::onConnectionStatusChanged);
    connect(&m_globalState, &GlobalState::currentProjectChanged, this, &MainWindow::onCurrentProjectChanged);
    connect(&m_globalState, &GlobalState::currentProjectPublished, this, &MainWindow::onCurrentProjectPublished);

    // for test only
    connect(&m_globalState, &GlobalState::new_test_buffer_arrived, this, &MainWindow::buffer_listener);


    ProjectModel* startingProject = new ProjectModel("Project");
    m_globalState.addProject(startingProject);
    m_globalState.setCurrentProject(startingProject);

    m_remoteInterface = new RemoteInterface(this);
    addToolBar(Qt::BottomToolBarArea, m_remoteInterface);

    setupRunService();
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
    // m_runService->setScene(m_scene);

    m_startBtn = new QPushButton("start", m_toolBar);
    // m_toolBar->addWidget(m_startBtn);
    m_stopBtn = new QPushButton("stop", m_toolBar);
    // m_toolBar->addWidget(m_stopBtn);
    m_timer = new QTimeEdit(m_toolBar);
    m_timer->setDisplayFormat("hh:mm:ss");
    m_timer->setFixedSize(120, 30);
    m_timer->setCurrentSection(QDateTimeEdit::MinuteSection);

    m_toolBar->addWidget(m_startBtn);
    m_toolBar->addWidget(m_stopBtn);
    m_toolBar->addWidget(m_timer);

    m_toolbar_blocker = new ActionsBlocker(m_toolBar);
    m_scene_blocker = new ActionsBlocker(m_view);
    m_scene_blocker->transparency(0.0);
    m_liveUpdate_forLogger = std::make_unique<LiveUpdate>(m_scene);
    m_DB_handler = new DB_handler();
    m_saveAndLoad = new saveAndLoad(&m_globalState);
    m_parser = new parser();

    clientSocket = new ClientSocket(100);
    auto func = [](ListenErrorCode error) {
        std::cout << "listen" << std::endl;
    };
    std::thread t([&func]() {
        while (true) {
            char buffer[MAXRECV];
            listenAsync(buffer, sizeof(buffer), func);
        }
    });

    t.join();

    onRunEnd();
    QObject::connect(m_runService.get(), &RunService::stopFinished, [this](){
        onRunEnd();
    });
    QObject::connect(m_runService.get(), &RunService::startBegin, [this](){

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
    m_publishButton = new QPushButton("Publish project", m_sceneBox);
    m_publishButton->setDisabled(m_globalState.connectionState() != globalConstants::ConnectionState::Online);
    QObject::connect(m_publishButton, &QPushButton::clicked, [this] {
        m_globalState.publishCurrentProject();
    });
    // set the style of the button
    m_publishButton->setStyleSheet("QPushButton { border: 1px solid black; border-radius: 5px; padding: 5px; }");
    

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
//    if (!m_currentFrameBackgroundPath.isEmpty()) {
//        QPixmap backgroundImage(m_currentFrameBackgroundPath);
//        QPixmap scaledImage = backgroundImage.scaled(mainFrame->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
//        QPalette palette;
//        palette.setBrush(QPalette::Window, scaledImage);
//        mainFrame->setAutoFillBackground(true);
//        mainFrame->setPalette(palette);
//    }
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
        m_mainLayout->addWidget(controlPanel);
        m_initializeSensorsData->initialize();
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
    // for test only
    m_bufferTest = new buffer_test(); // this generates buffer every 2 seconds, and write then to A.log
    // end text
    m_globalState.setIsRunning(true);
    m_startBtn->hide();
    m_timer->hide();

    m_stopBtn->show();
    //m_toolbar_blocker->show();
    //m_scene_blocker->show();
    m_stopBtn->raise();
}

void MainWindow::onRunEnd()
{
    m_startBtn->show();
    m_timer->show();

    m_stopBtn->hide();
    m_toolbar_blocker->hide();
    m_scene_blocker->hide();
}


//ClientSocket client(id);
// use listen asynchronously
// char buffer[MAXRECV];
// auto func = [](ListenErrorCode){std::cout << "listen" << std::endl; };
// client.listenAsync(buffer , sizeof(buffer),func);
// std::string mm = buffer;

// for test only
void MainWindow:: buffer_listener(const QString& data) {
    m_globalState.newData(data);
}

void MainWindow:: listener(){
//    ClientSocket client(100);
////    use listen asynchronously
//    char buffer[MAXRECV];
//    auto func = [](ListenErrorCode){std::cout << "listen" << std::endl; };
//    client.listenAsync(buffer , sizeof(buffer),func);
//    QString data = buffer;
//    m_globalState.newData(data);
}

void MainWindow::saveLayout() {
    m_globalState.saveData();
}
void MainWindow::loadLayout() {
    m_globalState.loadData();
}
