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
    if (!m_logFile.open(QIODevice::Append | QIODevice::Text)) {

//        if (!m_logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open log file for reading:" << m_logFile.errorString();
        return;
    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &LogReader::readNewLogEntries);
    m_timer->start(5000); // Check for new log entries every 5 seconds
}
void LogReader::generate_buffer() {
    // Create a demo buffer
    QByteArray buffer;
    buffer.resize(200);
    buffer.fill(0);

    // Generate random values for the first 7 bits and the second set of 7 bits
    wint_t id = rand() % 3 + 1; // Random value between 1 and 3
    wint_t dest_id = (id + rand() % 2 + 1) % 3 + 1; // Different from 'id'

    // Store 'id' in bits 0 to 6
    buffer[0] = static_cast<char>(id & 0x7F);
    QString randomMessage = msg[msg_counter % 5];
    QByteArray message = randomMessage.toLatin1();
    int messageLength = qMin(message.length(), 128);
    buffer.replace(1, messageLength, message);

    // Store 'dest_id' in bits 136 to 142 using the next available byte
    buffer[129] = static_cast<char>(dest_id & 0x7F);

    // Generate a timestamp (replace with your actual timestamp logic)
    QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

//    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    QByteArray timestampBytes = timestamp.toLatin1();
    buffer.replace(176, timestampBytes.length(), timestampBytes);

    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
        out << buffer << "\n";
    }
    msg_counter++;
}


    // Print the entire buffer
//    wint_t exit_id = buffer[0] & 0x7F;
//    QString exit_message = QString::fromLatin1(buffer.mid(1, 128).constData());
//    wint_t exit_dest_id = buffer[129] & 0x7F;
//
//    qInfo() << "Generated data for buffer:";
//    qInfo() << "ID:" << exit_id << "(ASCII: '" << QChar(exit_id) << "')";
//    qInfo() << "Message:" << exit_message;
//    qInfo() << "Destination ID:" << exit_dest_id;

//    dbHandler->write_to_DB(buffer);
    // End test
//}


//void LogReader::generate_buffer(){
//    // just for test
//    // create demo buffer
//
//    QByteArray buffer;
//    buffer.resize(200);
//    buffer.fill(0);
//
//    wint_t id = 5; // 7-bit unsigned integer
//    QByteArray message = "yossi goldberg the best";
//    wint_t dest_id = 17; // 7-bit unsigned integer
//    // Store 'id' in bits 0 to 6
//    buffer[0] = static_cast<char>(id & 0x7F);
//    // Store the message in bits 8 to 135 (using buffer starting from index 1)
//    int messageLength = qMin(message.length(), 128);
//    buffer.replace(1, messageLength, message);
//    // Store 'dest_id' in bits 136 to 142 using the next available byte
//    buffer[129] = static_cast<char>(dest_id & 0x7F);
//    // Print the entire buffer
//    wint_t exit_id = buffer[0] & 0x7F;
//    // Extract message (bits 8 to 135)
//    QString exit_message = QString::fromLatin1(buffer.mid(1, 128).constData());
//    // Extract dest_id (bits 136 to 142)
//    wint_t exit_dest_id = buffer[129] & 0x7F;
////    Print extracted data
//    qInfo() << "Extracted data from buffer:";
//    qInfo() << "ID:" << exit_id << "(ASCII: '" << QChar(exit_id) << "')";
//    qInfo() << "Message:" << exit_message;
//    qInfo() << "Destination ID:" << exit_dest_id;
//
//
//    dbHandler->write_to_DB(buffer);
//    // end test
//}

void LogReader::readNewLogEntries() { return;
    if (!m_logFile.isOpen()) {
        return;
    }
    m_logFile.seek(m_lastPosition);
    QTextStream in(&m_logFile);
    generate_buffer();
    while (!in.atEnd()) {
        QByteArray line = in.readLine().toUtf8();
        if(m_simulationRecorder){
            m_simulationRecorder->recordEvent(line);
        }
        qInfo() << "new_log";
        dbHandler->write_to_DB(line);

//        m_LiveUpdate->parse_new_line(line);
    }
    m_lastPosition = m_logFile.pos();
}

