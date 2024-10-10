//
// Created by OWNER on 06/08/2024.
//

#ifndef VEHICLECOMPSIM_SIMULATIONRECORDER_H
#define VEHICLECOMPSIM_SIMULATIONRECORDER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QObject>




class SimulationRecorder :public QObject{
Q_OBJECT
public:
    SimulationRecorder(const QString &filePath, const QString &db_file_path);

public slots:
    void copyDataToFile();
    void addStartTime();
private:
    QFile m_logFile;
    QString db_path;
    bool first_running = true;
};




#endif //VEHICLECOMPSIM_SIMULATIONRECORDER_H
