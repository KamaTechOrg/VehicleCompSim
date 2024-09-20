#pragma once
#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <memory>
#include <QtWidgets/QPushButton>
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
#include "qpushbutton.h"
#include "services/runservice.h"
#include "SimulationRecorder.h"
#include "SimulationReplayer.h"
#include "LiveUpdate.h"
#include "LogReader.h"
#include "SimulationControlPanel.h"
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

class QGraphicsView;
class QToolBar;

class MainWindow : public QMainWindow {
Q_OBJECT

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

private slots:
    void onOnlineStatusChanged(bool online);
    void onCurrentProjectChanged(ProjectModel* project);
    void onCurrentProjectPublished(ProjectModel* project);
    void close_previous_replay();
    void update_view();
//    void fill_db_data();
//    void read_from_json();
//    void fill_box_data();

private:
    CustomScene* m_scene;
    QGraphicsView* m_view;
    GlobalState &m_globalState;

    QToolBar* m_toolBar;
    QToolBar* rightToolBar;
    PopupDialog* m_popupDialog;
    ActionsBlocker* m_toolbar_blocker;
    ActionsBlocker* m_scene_blocker;
    QPushButton *startBtn;
    QPushButton *stopBtn ;
    QTimeEdit *timer;
    std::shared_ptr<RunService> m_runService;
    std::unique_ptr<LogReader> m_logReader;
    std::unique_ptr<SimulationRecorder> m_simulationRecorder = nullptr;
    std::unique_ptr<SimulationReplayer> m_simulationReplayer;
    std::unique_ptr<LiveUpdate> m_liveUpdate_forLogger;
    std::unique_ptr<LiveUpdate> m_liveUpdate_forReplyer;
    SimulationControlPanel* controlPanel = nullptr;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_centerLayout;
    DB_handler *m_DB_handler;
    QTimer *change_view_timer;
    QJsonArray itemsArray;
    QLabel* m_connectionStatusLabel;
    QFrame* mainFrame;
    RemoteInterface* m_remoteInterface;
    buffer_test *m_bufferTest;
    initializeSensorsData *m_initializeSensorsData;

    QGroupBox* m_sceneBox;
    QPushButton* m_publishButton;
};
