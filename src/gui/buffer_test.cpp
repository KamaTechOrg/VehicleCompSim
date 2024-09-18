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
    m_timer->start(2000);
}
buffer_test::~buffer_test() {
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
    delete m_timer;
}

void buffer_test::generate_buffer() {
    int choosen_src = rand() % 10 + 1;;
    int choosen_dest;
    if (choosen_src < 5){
        choosen_dest = 5;
    }else if(choosen_src == 5 || choosen_src == 10){
        choosen_dest = 11;
    }else{
        choosen_dest = 10;
    }
    QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    QByteArray buffer;
    buffer.resize(50);
    buffer.fill(0);
    int num = my_map[choosen_src];
    int pos = 0;
    for (int i = 0; i < num; ++i) {
        QString randomMessage = msg[QRandomGenerator::global()->bounded(msg.size())];
        QByteArray message = randomMessage.toLatin1();
        int messageLength = qMin(message.length(), 10);
        buffer.replace(pos, messageLength, message);
        pos += 10;
    }
    QString new_line = timestamp + "," + QString::number(choosen_src) + "," + QString::number(choosen_dest) + "," + QString::number(50) + ",";
    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
        out << new_line << buffer << "\n";
    }
    msg_counter++;
}

