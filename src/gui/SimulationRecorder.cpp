//
// Created by OWNER on 06/08/2024.
//
#include "SimulationRecorder.h"

SimulationRecorder::SimulationRecorder(const QString &filePath) : m_logFile(filePath) {
    if (!m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Cannot open log file for writing:" << m_logFile.errorString();
    }
    QTextStream out(&m_logFile);
    out << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n";
}

void SimulationRecorder::recordEvent(const QString &event) {
    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
        out << QDateTime::currentDateTime().toString(Qt::ISODate) << "," << event << "\n";
    }
}



