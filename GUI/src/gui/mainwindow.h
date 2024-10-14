#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QTimer>
#include <QGraphicsView>
#include <QWidget>
#include <QObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QLabel>
#include <QFrame>
#include <QString>
#include <unordered_map>

#include "customscene.h"
#include "state/globalstate.h"
#include "items/actionsblocker.h"
#include "qdatetimeedit.h"
#include "services/runservice/runservice.h"
#include "SimulationRecorder.h"
#include "SimulationReplayer.h"
#include "remoteinterface.h"
#include "popupdialog.h"
#include "DB_handler.h"
#include "buffer_test.h"
#include "SimulationControlPanel.h"
#include "./editpanel.h"
#include "customwidget.h"
#include "../src/client/websocketclient.h"
#include "../src/client/handlers/RunHandler.h"
#include "items/qemusensoritem.h"
#include "services/parser.h"
#include "initializeSensorsData.h"
#include "../models/sensormodel.h"
#include "saveAndLoad.h"
#include "widgets/iconbutton.h"
#include "../../MainComputer/src/maincomputer.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    void background_Layout();
    void saveLayout(const QString &dirPath = QString());
    void loadLayout(const QString &dirPath = QString());
    void setupToolBar();
    void setupRunService();
    void setupView();
    void onRunStart(QString com_server_ip = "");
    void onRunEnd();
    void record();
    void replayer();
    void updateBackground();
    void resizeEvent(QResizeEvent* event) override;
    void buffer_listener(const QString& data);
    void handleProjectConnections(ProjectModel* newProject);
    void removeAllTabs();


private slots:
    void onConnectionStatusChanged(globalConstants::ConnectionState state);
    void onCurrentProjectChanged(ProjectModel* project);
    void onCurrentProjectPublished(ProjectModel* project);
    void close_previous_replay();
    void updateTimer();
    void handleNewLog(const QString &newLog, const QString &tabName);
    void createNewTab(SerializableItem* model);
    void updateTab(const QString &tabName, const QString & oldTabName);

    void removeTab(SerializableItem* model);
    void pressOnTab(const QString & tabName);
    void resetTabContent();

    void onModelAdded(SerializableItem* model);
    void onModelRemoved(SerializableItem* model);

private:
    CustomScene* m_scene;
    QGraphicsView* m_view;
    GlobalState &m_globalState;
    QString m_mainWindowTitle;
    QToolBar* m_toolBar;
    QToolBar* rightToolBar;
    ActionsBlocker* m_toolbar_blocker;
    ActionsBlocker* m_scene_blocker;
    QStackedWidget* m_buttonStack;
    IconButton *m_startBtn;
    IconButton *m_stopBtn ;
    QTimeEdit *m_timer;
    QTimer* m_countdownTimer;
    std::shared_ptr<RunService> m_runService;
    SimulationRecorder * m_simulationRecorder = nullptr;
    SimulationReplayer * m_simulationReplayer = nullptr;
    SimulationControlPanel* controlPanel = nullptr;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_centerLayout;
    DB_handler *m_DB_handler = nullptr;
    QLabel* m_connectionStatusLabel;
    QFrame* mainFrame;
    RemoteInterface* m_remoteInterface;
    buffer_test *m_bufferTest;
    initializeSensorsData *m_initializeSensorsData;
    QString m_currentMainBackgroundPath;
    QGroupBox* m_sceneBox;
    IconButton* m_publishButton;
    QWidget* m_layoutWidget;
    saveAndLoad *m_saveAndLoad;
    parser * m_parser;
    MainComputer mainComputer;

    QTabWidget* tabWidget;
    std::unordered_map<QString, QTextEdit*> textEditMap;
    std::unordered_map<QString, QWidget*> tabMap;
    std::unordered_map<QString, int> tabIndexMap;

    ProjectModel* m_currentProject = nullptr;

};

