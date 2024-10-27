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
                m_startTime = QDateTime::fromString(parts[0], "yyyy-MM-ddTHH:mm:ss.zzz");
                first_line = false;
                continue;
            }
            if (parts.size() == 5) {
                QDateTime timestamp = QDateTime::fromString(parts[0], "yyyy-MM-ddTHH:mm:ss.zzz");
                int delay = qMax(0, m_startTime.msecsTo(timestamp));
                QPair<QString, int> new_pair(line, delay);
                events.push_back(new_pair);
                m_totalTime = timestamp;
            }
        }
        m_logFile.close();
        scheduleEvents();
    }
}

void SimulationReplayer::scheduleEvents() {
    for(const auto &event : events){
        m_messagesQueue.enqueue(event.first);
        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, &SimulationReplayer::processMessage);
        m_timers.enqueue(timer);
        timer->start(event.second);
    }
}

void SimulationReplayer::processMessage() {
    if (!m_messagesQueue.isEmpty()) {
        QTimer * timer = m_timers.dequeue();
        timer->deleteLater();
        QString message = m_messagesQueue.dequeue();
        GlobalState::getInstance().newRecordData(message);
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
    m_remainingTimes.clear();
}

void SimulationReplayer::pauseSimulation() {
    for(auto timer : m_timers) {
        if (timer->isActive()) {
            m_remainingTimes[timer] = timer->remainingTime();
            timer->stop();
        }
    }
}

void SimulationReplayer::playSimulation() {
    for(auto timer : m_timers) {
        if (!timer->isActive() && m_remainingTimes.contains(timer)) {
            timer->start(m_remainingTimes[timer]);
        }
    }
}

void SimulationReplayer::scheduleEvent(const QString &line, int delay) {
    m_messagesQueue.enqueue(line);
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &SimulationReplayer::processMessage);
    m_timers.enqueue(timer);
    timer->start(delay);
}

void SimulationReplayer::jumpToTime(const QTime &targetTime) {
    GlobalState::getInstance().delTabContent();
    clear_current_events();
    int msecsToAdd = QTime(0, 0).msecsTo(targetTime);
    for (const auto &event : events) {
        int remainingTime = event.second - msecsToAdd;
        if (remainingTime <= 0) {
            GlobalState::getInstance().newRecordData(event.first);
        } else {
            scheduleEvent(event.first, remainingTime);
        }
    }
}
