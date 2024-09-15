//
// Created by OWNER on 06/08/2024.
//

#include "LogReader.h"
#include <QDebug>
#include <bitset>

LogReader::LogReader(const QString &logFilePath, std::unique_ptr<LiveUpdate> liveUpdate, DB_handler *db,
                     std::unique_ptr<SimulationRecorder> simulationRecorder, QObject *parent)
    : QObject(parent), m_logFile(logFilePath), m_lastPosition(0), m_timer(new QTimer(this)),
    m_LiveUpdate(std::move(liveUpdate)), dbHandler(db), m_simulationRecorder(std::move(simulationRecorder)) {
    if (!m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open log file for reading:" << m_logFile.errorString();
        return;
    }
//    qInfo() << "log reader activate";
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &LogReader::readNewLogEntries);
    m_timer->start(5000); // Check for new log entries every 5 seconds
}

void LogReader::readNewLogEntries() {
//    qInfo() << "read log";
    if (!m_logFile.isOpen()) {
        return;
    }
    m_logFile.seek(m_lastPosition);
    QTextStream in(&m_logFile);
    while (!in.atEnd()) {
        QByteArray line = in.readLine().toUtf8();
//        qDebug() << "Line length:" << line.length();
        if(line.length() != 200){
            continue;
        }
//        qInfo() << "enter to while";
//        QByteArray line = in.readLine().toUtf8();
//        if(m_simulationRecorder){
//            m_simulationRecorder->recordEvent(line);
//        }
//        qInfo() << "new_log";
        dbHandler->write_to_DB(line);

//        m_LiveUpdate->parse_new_line(line);
    }
//    qInfo() << m_lastPosition;
    m_lastPosition = m_logFile.pos();
//    qInfo() << m_lastPosition;
}

