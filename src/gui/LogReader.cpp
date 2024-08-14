//
// Created by OWNER on 06/08/2024.
//

#include "LogReader.h"
#include <QDebug>

LogReader::LogReader(const QString &logFilePath, std::unique_ptr<LiveUpdate> liveUpdate, std::unique_ptr<SimulationRecorder> simulationRecorder, QObject *parent)
        : QObject(parent), m_logFile(logFilePath), m_lastPosition(0), m_timer(new QTimer(this)), m_LiveUpdate(std::move(liveUpdate)), m_simulationRecorder(std::move(simulationRecorder)) {

    if (!m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open log file for reading:" << m_logFile.errorString();
        return;
    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &LogReader::readNewLogEntries);
    m_timer->start(5000); // Check for new log entries every 5 seconds
}

void LogReader::readNewLogEntries() {
    if (!m_logFile.isOpen()) {
        return;
    }

    m_logFile.seek(m_lastPosition);
    QTextStream in(&m_logFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(m_simulationRecorder){
            m_simulationRecorder->recordEvent(line);
        }
        m_LiveUpdate->parse_new_line(line);
    }
    m_lastPosition = m_logFile.pos();
}