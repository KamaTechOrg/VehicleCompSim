//
// Created by OWNER on 06/08/2024.
//

#ifndef VEHICLECOMPSIM_SIMULATIONREPLAYER_H
#define VEHICLECOMPSIM_SIMULATIONREPLAYER_H

#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QQueue>
#include "customscene.h"
#include "LiveUpdate.h"
#include "DB_handler.h"
#include "customscene.h"

class SimulationReplayer : public QObject {
Q_OBJECT

public:
    SimulationReplayer(const QString &filePath);
    void pauseSimulation();
    void playSimulation();
    void jumpToTime(const QTime &targetTime);
    void startReplay();
    QDateTime start() const { return m_startTime; }
    void clear_current_events();


private:
    void scheduleEvent(const QString &event, int delay);

private slots:
    void processEvent();

private:
    QFile m_logFile;
    qint64 m_lastPosition;
    QQueue<QString> m_eventQueue;
    QList<QTimer *> m_timers;
    std::unique_ptr<LiveUpdate> m_LiveUpdate;
    QDateTime m_currentTime;
    DB_handler *m_db;
    CustomScene* m_scene_simulation;
public:
    QDateTime m_startTime;
    QDateTime m_totalTime;
};



#endif //VEHICLECOMPSIM_SIMULATIONREPLAYER_H
