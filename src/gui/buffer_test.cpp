//
// Created by OWNER on 15/09/2024.
//

#include "buffer_test.h"

buffer_test::buffer_test(QObject *parent) : QObject(parent), m_logFile(QDir::currentPath() + "/A.log"), msg_counter(0) {
    if (m_logFile.exists()) {
        if (!m_logFile.remove()) {
            qDebug() << "Could not delete existing log file:" << m_logFile.errorString();
            return;
        }
    }
    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Error opening the log file for reading and writing.";
        return;
    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &buffer_test::generate_buffer);
    m_timer->start(2000); // Check for new log entries every 5 seconds
}
buffer_test::~buffer_test() {
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
    delete m_timer;
}

void buffer_test::generate_buffer() {
    // Create a demo buffer
    QByteArray buffer;
    buffer.resize(200);
    buffer.fill(0);

    wint_t id = rand() % 10 + 1; // Random value between 1 and 10
//    qInfo() << "id" << id;
    wint_t dest_id;
    do {
        dest_id = rand() % 10 + 1;
    } while (dest_id == id); // Ensure dest_id is different from id
//    qInfo() << "dest " << dest_id;

    buffer[0] = static_cast<char>(id & 0x7F);
    wint_t exit_id = buffer[0] & 0x7F;
//    qInfo() << "exit id" << exit_id;

    QString randomMessage = msg[msg_counter % 5];
    QByteArray message = randomMessage.toLatin1();
    int messageLength = qMin(message.length(), 30);
    buffer.replace(1, messageLength, message);
    QString exit_message = QString::fromLatin1(buffer.mid(1, 30).constData());
//    qInfo() << "Message:" << exit_message;

    buffer[31] = static_cast<char>(dest_id & 0x7F);
    wint_t dest_exit_id = buffer[31] & 0x7F;
//    qInfo() << "dest_exit_id" << dest_exit_id;

    QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    QByteArray timestampBytes = timestamp.toLatin1();
    buffer.replace(176, timestampBytes.length(), timestampBytes);
//    QString time_stamp = QString::fromLatin1(buffer.mid(200 - 24, 200).constData());
//    qInfo() << "time_stamp" << time_stamp;

    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
//        qInfo() << "out.pos before insertions" << out.pos();
        out << buffer << "\n";
//        qInfo() << "out.pos after insertions" << out.pos();
    }
    msg_counter++;
}
