//
// Created by OWNER on 06/08/2024.
//

#include "SimulationReplayer.h"
#include "globalstate.h"
#include <QDebug>

SimulationReplayer::SimulationReplayer(const QString &filePath) : m_logFile(filePath){
    startReplay();
}

void SimulationReplayer::startReplay() {
    if (m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
        m_logFile.close();
    }
}

void SimulationReplayer::scheduleEvent(const QString &message, int delay) {
    m_messagesQueue.enqueue(message);
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &SimulationReplayer::processMessage);
    m_timers.enqueue(timer);
    timer->start(delay);

}

void SimulationReplayer::processMessage() {
    if (!m_messagesQueue.isEmpty()) {
        QTimer * timer = m_timers.dequeue();
        timer->deleteLater();
        QString message = m_messagesQueue.dequeue();
        GlobalState::getInstance().newData(message);
    } else {
        qWarning() << "processMessage: Event queue is empty";
    }
}

void SimulationReplayer::clear_current_events(){
    m_messagesQueue.clear();
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
void SimulationReplayer::jumpToTime(const QTime &targetTime, bool isManualJump) {
    if (!isManualJump) {
        return;
    }
    int secondsToAdd = targetTime.hour() * 3600 + targetTime.minute() * 60 + targetTime.second();
    clear_current_events();
    if (m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
        m_logFile.close();
    }
}