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

class SimulationReplayer : public QObject {
Q_OBJECT

public:
    SimulationReplayer(const QString &filePath, std::unique_ptr<LiveUpdate> liveUpdate, QObject *parent = nullptr);

    void jumpToTime(const QDateTime &targetTime);
    void startReplay();

private:
    void clear_current_events();
    void scheduleEvent(const QString &event, int delay);

private slots:

    void processEvent();



private:
    QFile m_logFile;
    qint64 m_lastPosition;
    QQueue<QString> m_eventQueue;
    QList<QTimer *> m_timers;
    std::unique_ptr<LiveUpdate> m_LiveUpdate;
    QDateTime start;
};




#endif //VEHICLECOMPSIM_SIMULATIONREPLAYER_H
