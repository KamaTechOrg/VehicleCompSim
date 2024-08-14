//
// Created by OWNER on 06/08/2024.
//

#ifndef VEHICLECOMPSIM_LOGREADER_H
#define VEHICLECOMPSIM_LOGREADER_H


#include <QObject>
#include <QFile>
#include <QTimer>
#include <memory>
#include "CustomScene.h"
#include "SimulationRecorder.h"
#include "LiveUpdate.h"

class LogReader : public QObject {
Q_OBJECT
public:
    LogReader(const QString &logFilePath, std::unique_ptr<LiveUpdate> liveUpdate, std::unique_ptr<SimulationRecorder> simulationRecorder = nullptr, QObject *parent = nullptr);

private slots:
    void readNewLogEntries();
public:
    std::unique_ptr<SimulationRecorder> m_simulationRecorder;

private:
    QFile m_logFile;
    qint64 m_lastPosition;
    QTimer *m_timer;
    std::unique_ptr<LiveUpdate> m_LiveUpdate;
};


#endif //VEHICLECOMPSIM_LOGREADER_H
