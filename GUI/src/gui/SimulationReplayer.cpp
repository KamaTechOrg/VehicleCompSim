//
// Created by OWNER on 06/08/2024.
//

#include "SimulationReplayer.h"
#include "globalstate.h"
#include <QDebug>

SimulationReplayer::SimulationReplayer(const QString &filePath, DB_handler *db, std::unique_ptr<LiveUpdate> liveUpdate, CustomScene* m_scene, QObject *parent)
        : QObject(parent), m_db(db), m_logFile(filePath), m_LiveUpdate(std::move(liveUpdate)), m_lastPosition(0), m_scene_simulation(m_scene) {
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
            QByteArray line = in.readLine().toUtf8();
            QList<QByteArray> parts = line.split(',');
            if(first_line){
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                m_startTime = QDateTime::fromString(parts[0], Qt::ISODate);
                first_line = false;
                continue;
            }
            if (parts.size() == 6) {
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                int delay = qMax(0, m_startTime.msecsTo(timestamp));
                scheduleEvent(line, delay);
                m_totalTime = timestamp;
            }
        }
    }
}

void SimulationReplayer::scheduleEvent(const QByteArray &event, int delay) {
    m_eventQueue.enqueue(event);
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &SimulationReplayer::processEvent);
    timer->start(delay);
    m_timers.append(timer);

}

void SimulationReplayer::processEvent() {
    if (!m_eventQueue.isEmpty()) {
        QByteArray event = m_eventQueue.dequeue();
        QList<QByteArray> parts = event.split(',');
        m_currentTime = QDateTime::fromString(parts[0], Qt::ISODate);
        QByteArray event_without_time = parts.mid(1, 5).join(',');
        m_db->write_data_to_DB(event_without_time);
        // need to call just after all the junk of lines that theire timers ended already updated in the db
        // in this time update view is calling after every update in the db. not ok!
        update_view();
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
            QByteArray line = in.readLine().toUtf8();
            if(first_line){
                first_line = false;
                continue;
            }
            QList<QByteArray> parts = line.split(',');
            if (parts.size() == 6) {
                QDateTime timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
                if (timestamp > m_startTime.addSecs(secondsToAdd)) {
                    int delay = qMax(0, m_startTime.addSecs(secondsToAdd).msecsTo(timestamp));
                    scheduleEvent(line, delay);
                }else{
                    QByteArray event_without_time = parts.mid(1, 5).join(',');
                    m_db->write_data_to_DB(event_without_time);
                    update_view();
                }
            }
        }
    }
}
void SimulationReplayer::update_view() {
//    auto models = GlobalState::getInstance().currentProject()->models();
////    QMap<QString, QVariantList> last_changes;
//    for (auto model: models) {
//        if (auto *sensor = dynamic_cast<SensorModel *>(model)) {
//            QString sensorId = sensor->priority();
//            QList<QVariant> data = m_db->read_all_sensor_data(sensorId);
//            GlobalState::getInstance().parsedData(sensorId, data);
//
////            last_changes[sensorId] = data;
//        }
//    }
//    m_liveUpdate_forLogger->parse_new_data(last_changes);
}

//void SimulationReplayer::update_view() {
//    QMap<QString, QVariantList> last_changes;
//    for (auto item : m_scene_simulation->items()) {
//        if (auto *sensor = dynamic_cast<SensorItem *>(item)) {
//            QString sensorId = sensor->getPriority();
//            QList<QVariant> data = m_db->read_all_sensor_data(sensorId);
////            QList<QVariant> data = m_db->read_last_from_DB(sensorId);
//            sensor->update_data(data);
//            last_changes[sensorId] = data;
//        }
//    }
//    m_LiveUpdate->parse_new_data(last_changes);
//
//}
