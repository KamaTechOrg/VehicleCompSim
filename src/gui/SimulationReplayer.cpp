//
// Created by OWNER on 06/08/2024.
//

#include "SimulationReplayer.h"
#include <QDebug>

SimulationReplayer::SimulationReplayer(const QString &filePath, std::unique_ptr<LiveUpdate> liveUpdate, QObject *parent)
        : QObject(parent), m_logFile(filePath), m_LiveUpdate(std::move(liveUpdate)), m_lastPosition(0) {
    if (!m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open log file for reading:" << m_logFile.errorString();
    }
}

void SimulationReplayer::startReplay() {
    if (m_logFile.isOpen()) {
        m_logFile.seek(m_lastPosition); // Move to the last read position
        QTextStream in(&m_logFile);
        bool first_line = true;
        while (!in.atEnd()) {

            QString line = in.readLine();
            if(first_line){
                start = QDateTime::fromString(line, Qt::ISODate);
                first_line = false;
                continue;
            }
            QStringList parts = line.split(',');
            if (parts.size() == 4) {
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                QString event = parts.mid(1, 3).join(',');
                QStringList communication_data = event.split(',');
                int delay = qMax(0, start.msecsTo(timestamp));
                scheduleEvent(event, delay);
            }else{
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
        m_LiveUpdate->parse_new_line(event);
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

void SimulationReplayer::jumpToTime(const QDateTime &targetTime) {
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
            QStringList parts = line.split(',');
            if (parts.size() == 2) {
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                if (timestamp > targetTime) {
                    QString event = parts.mid(1, 3).join(',');
                    int delay = qMax(0, start.msecsTo(timestamp));
                    scheduleEvent(event, delay);
                }
            }
        }
    }
}
