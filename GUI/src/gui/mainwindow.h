#pragma once
#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <memory>
#include <QtWidgets/QTimeEdit>
#include <QLineEdit>
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
#include <QRect>
#include <QHBoxLayout>

#include "customscene.h"
#include "items/actionsblocker.h"
#include "qdatetimeedit.h"
#include "services/runservice/runservice.h"
#include "SimulationRecorder.h"
#include "SimulationReplayer.h"
#include "LiveUpdate.h"
#include "LogReader.h"
#include "remoteinterface.h"
#include <bson/bson.h>
#include "popupdialog.h"
#include "DB_handler.h"
#include "buffer_test.h"
#include "SimulationControlPanel.h"
#include "./editpanel.h"
#include "customwidget.h"
#include "websocketclient.h"
#include "handlers/RunHandler.h"
#include "globalstate.h"
#include "items/qemusensoritem.h"
#include "initializeSensorsData.h"
#include "sensormodel.h"
#include "../../../Communication/server/include/manger.h"
#include "saveAndLoad.h"
#include "items/parser.h"
#include "globalconstants.h"

class QGraphicsView;
class QToolBar;

class MainWindow : public QMainWindow {
Q_OBJECT
    MangServer server;
public:
    MainWindow(QWidget* parent = nullptr);

private:
    void background_Layout();
    void saveLayout();
    void loadLayout();
    void setupToolBar();
    void setupRunService();
    void setupView();
    void onRunStart();
    void onRunEnd();
    void record();
    void replayer();
    void create_sensor_from_bson_obj(const bson_t *bsonDocument);
    void updateBackground();
    void resizeEvent(QResizeEvent* event) override;
    void buffer_listener(const QString& data);
private slots:
    void onConnectionStatusChanged(globalConstants::ConnectionState state);
    void onCurrentProjectChanged(ProjectModel* project);
    void onCurrentProjectPublished(ProjectModel* project);
    void close_previous_replay();
//    void update_view();
//    void fill_db_data();
//    void read_from_json();
//    void fill_box_data();

private:
    CustomScene* m_scene;
    QGraphicsView* m_view;
    GlobalState &m_globalState;

    QString m_mainWindowTitle;
    
    QToolBar* m_toolBar;
    QToolBar* rightToolBar;
    PopupDialog* m_popupDialog;
    ActionsBlocker* m_toolbar_blocker;
    ActionsBlocker* m_scene_blocker;
    QPushButton *m_startBtn;
    QPushButton *m_stopBtn ;
    QTimeEdit *m_timer;
    std::shared_ptr<RunService> m_runService;
//    std::unique_ptr<LogReader> m_logReader;
    SimulationRecorder * m_simulationRecorder = nullptr;
    SimulationReplayer * m_simulationReplayer = nullptr;
    std::unique_ptr<LiveUpdate> m_liveUpdate_forLogger;
    std::unique_ptr<LiveUpdate> m_liveUpdate_forReplyer;
    SimulationControlPanel* controlPanel = nullptr;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_centerLayout;
    DB_handler *m_DB_handler;
//    QTimer *change_view_timer;
    QJsonArray itemsArray;
    QLabel* m_connectionStatusLabel;
    QFrame* mainFrame;
    RemoteInterface* m_remoteInterface;
    buffer_test *m_bufferTest;
    initializeSensorsData *m_initializeSensorsData;
    QString m_currentMainBackgroundPath;

    QGroupBox* m_sceneBox;
    QPushButton* m_publishButton;
    QWidget* m_layoutWidget;
    saveAndLoad *m_saveAndLoad;
    parser * m_parser;
};

