//
// Created by OWNER on 06/08/2024.
//

#ifndef VEHICLECOMPSIM_SIMULATIONRECORDER_H
#define VEHICLECOMPSIM_SIMULATIONRECORDER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

class SimulationRecorder {
public:
    SimulationRecorder(const QString &filePath);

    void recordEvent(const QString &event);

private:
    QFile m_logFile;
};




#endif //VEHICLECOMPSIM_SIMULATIONRECORDER_H
