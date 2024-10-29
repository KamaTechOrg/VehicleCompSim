//
// Created by OWNER on 06/08/2024.
//
#include <QCoreApplication>
#include "SimulationRecorder.h"
#include "globalstate.h"
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>

SimulationRecorder::SimulationRecorder(const QString &filePath, const QString &db_file_path)
    : m_logFile(filePath), db_path(db_file_path) {

    connect(&GlobalState::getInstance(), &GlobalState::isRunningChanged, this, &SimulationRecorder::addStartTime);
    connect(qApp, &QCoreApplication::aboutToQuit, this, &SimulationRecorder::copyDataToFile);
}

void SimulationRecorder::addStartTime() {
    if(first_running){
        first_running = false;
        if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&m_logFile);
            out << QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz") << "\n";
            m_logFile.close();
        }
    }
}

void SimulationRecorder::copyDataToFile() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path);

    if (!db.open()) {
        qWarning() << "Unable to open database:" << db.lastError().text();
        return;
    }

    QSqlQuery query("SELECT * FROM DB");
    if (query.exec()) {
        if (m_logFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&m_logFile);
            while (query.next()) {
                int fieldCount = query.record().count();
                for (int i = 0; i < fieldCount; ++i) {
                    out << query.value(i).toString();
                    if (i < fieldCount - 1) {
                        out << ",";
                    }
                }
                out << "\n";
            }
            m_logFile.close();
            qDebug() << "Data copied to file successfully!";
        } else {
            qWarning() << "Cannot open log file for appending:" << m_logFile.errorString();
        }
    } else {
        qWarning() << "Error executing query:" << query.lastError().text();
    }

    db.close();
}



