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
signals:
    void delAllTabContent();
private:
    void scheduleEvents();
    void scheduleEvent(const QString &line, int delay);
private slots:
    void processMessage();

private:
    QFile m_logFile;
    QQueue<QString> m_messagesQueue;
    QQueue<QTimer *> m_timers;
    QDateTime m_currentTime;
    QList<QPair<QString, int>> events;

public:
    QDateTime m_startTime;
    QDateTime m_totalTime;
};

#endif //VEHICLECOMPSIM_SIMULATIONREPLAYER_H