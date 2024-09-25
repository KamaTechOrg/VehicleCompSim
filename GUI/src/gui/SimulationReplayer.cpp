//
// Created by OWNER on 06/08/2024.
//

#include "SimulationReplayer.h"
#include "globalstate.h"
#include <QDebug>

SimulationReplayer::SimulationReplayer(const QString &filePath) : m_logFile(filePath){
    if (!m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open log file for reading:" << m_logFile.errorString();
    }
}

void SimulationReplayer::startReplay() {
    if (m_logFile.isOpen()) {
        m_logFile.seek(m_lastPosition);
        QTextStream in(&m_logFile);
        bool first_line = true;
        while (!in.atEnd()) {
            QString line = in.readLine();
            QList<QString> parts = line.split(',');
            if(first_line){
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                m_startTime = QDateTime::fromString(parts[0], Qt::ISODate);
                first_line = false;
                continue;
            }
            if (parts.size() == 5) {
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                int delay = qMax(0, m_startTime.msecsTo(timestamp));
                scheduleEvent(line, delay);
                m_totalTime = timestamp;
            }
        }
    }
}

void SimulationReplayer::scheduleEvent(const QString &event, int delay) {
    m_eventQueue.enqueue(event);
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &SimulationReplayer::processEvent);
    timer->start(delay);
    m_timers.append(timer);

}

void SimulationReplayer::processEvent() {
    if (!m_eventQueue.isEmpty()) {
        QString event = m_eventQueue.dequeue();
        QList<QString> parts = event.split(',');
        m_currentTime = QDateTime::fromString(parts[0], Qt::ISODate);
        GlobalState::getInstance().newData(event);
    } else {
        qWarning() << "processEvent: Event queue is empty";
    }
}

void SimulationReplayer::clear_current_events(){
    m_eventQueue.clear();
    for (QTimer *timer : m_timers) {
        timer->stop();
        timer->deleteLater();
    }
    m_timers.clear();
}
void SimulationReplayer::pauseSimulation() {
    for(auto timer : m_timers){
        timer->stop();
    }
}
void SimulationReplayer::playSimulation() {
    for(auto timer : m_timers){
        timer->start();
    }
}
void SimulationReplayer::jumpToTime(const QTime &targetTime) {
    int secondsToAdd = targetTime.hour() * 3600 + targetTime.minute() * 60 + targetTime.second();
    clear_current_events();
    if (m_logFile.isOpen()) {
        m_logFile.seek(0);
        QTextStream in(&m_logFile);
        bool first_line = true;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if(first_line){
                first_line = false;
                continue;
            }
            QList<QString> parts = line.split(',');
            if (parts.size() == 5) {
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                if (timestamp > m_startTime.addSecs(secondsToAdd)) {
                    int delay = qMax(0, m_startTime.addSecs(secondsToAdd).msecsTo(timestamp));
                    scheduleEvent(line, delay);
                }else{
                    GlobalState::getInstance().newData(line);
                }
            }
        }
    }
}
