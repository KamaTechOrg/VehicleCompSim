//
// Created by OWNER on 06/08/2024.
//

#ifndef VEHICLECOMPSIM_LOGREADER_H
#define VEHICLECOMPSIM_LOGREADER_H


#include <QObject>
#include <QFile>
#include <QTimer>
#include <memory>
#include "customscene.h"
#include "SimulationRecorder.h"
#include "LiveUpdate.h"
#include "DB_handler.h"


class LogReader : public QObject {
Q_OBJECT
public:
    LogReader(const QString &logFilePath, DB_handler *db, std::unique_ptr<SimulationRecorder> simulationRecorder = nullptr, QObject *parent = nullptr);

private slots:
    void readNewLogEntries();
public:
    std::unique_ptr<SimulationRecorder> m_simulationRecorder;


private:
    QFile m_logFile;
    qint64 m_lastPosition;
    QTimer *m_timer;
    DB_handler *dbHandler;
    QList<QString> msg = {"yossi goldebrg", "naomi goldberg", "daniel goldebrg", "nechemia goldebrg", "avishay goldebrg"};
    int msg_counter = 0;
};


#endif //VEHICLECOMPSIM_LOGREADER_H
