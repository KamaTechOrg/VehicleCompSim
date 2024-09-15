#pragma once
#include <QtWidgets/QMainWindow>

#include <QMainWindow>
#include <memory>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>

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
    void onRunStart();
    void onRunEnd();
    void record();
    void replayer();
    void create_sensor_from_bson_obj(const bson_t *bsonDocument);

private slots:
    void onConnectionStatusChanged(bool connected);
    void close_previous_replay();

private:
    CustomScene* m_scene;
    QGraphicsView* m_view;
    QToolBar* m_toolBar; 
    ActionsBlocker* m_toolbar_blocker;
    ActionsBlocker* m_scene_blocker;
    QPushButton *startBtn;
    QPushButton *stopBtn ;
    QTimeEdit *timer;
    std::shared_ptr<RunService> m_runService;
    std::unique_ptr<LogReader> m_logReader;
    std::unique_ptr<SimulationRecorder> m_simulationRecorder;
    std::unique_ptr<SimulationReplayer> m_simulationReplayer;
    std::unique_ptr<LiveUpdate> m_liveUpdate_forLogger;
    std::unique_ptr<LiveUpdate> m_liveUpdate_forReplyer;
    SimulationControlPanel* controlPanel = nullptr;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QLabel* m_connectionStatusLabel;
    QFrame* mainFrame;
    RemoteInterface* m_remoteInterface;
};
