//
// Created by OWNER on 09/09/2024.
//

#include <bitset>
#include "DB_handler.h"

DB_handler::DB_handler() {
    sqlitedb = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    dbFilePath = QDir::currentPath() + "/data.db";
    QFile file(dbFilePath);
    if (file.exists()) {
        if (!file.remove()) {
            qDebug() << "Could not delete existing database file:" << file.errorString();
            return;
        }
    }
    sqlitedb->setDatabaseName(dbFilePath);
    if (!sqlitedb->open()) {
        qDebug() << "Could not open the database:" << sqlitedb->lastError().text();
    }
    connect(&GlobalState::getInstance(), &GlobalState::newDataArrived, this, &DB_handler::write_data_to_DB);
}

void DB_handler::write_data_to_DB(const QString& data, size_t bufferSize) const {
    const QStringList columnInfo = {"time", "src_id", "dest_id", "len", "buffer"};
    QStringList pieces = data.split(',', Qt::SkipEmptyParts);
    if (!sqlitedb->isOpen() && !sqlitedb->open()) {
        qCritical() << "Database connection failed:" << sqlitedb->lastError().text();
        return;
    }
    QSqlQuery query(*sqlitedb);
    sqlitedb->transaction();
    const QString createTableQuery = QString(
            "CREATE TABLE IF NOT EXISTS DB (%1)"
    ).arg(columnInfo.join(" TEXT, ") + " TEXT");
    if (!query.exec(createTableQuery)) {
        qCritical() << "Failed to create table:" << query.lastError().text();
        sqlitedb->rollback();
        return;
    }
    const QString insertQuery = QString(
            "INSERT INTO DB (%1) VALUES (%2)"
    ).arg(columnInfo.join(", "), QStringList(columnInfo.size(), "?").join(", "));

    if (!query.prepare(insertQuery)) {
        qCritical() << "Failed to prepare insert query:" << query.lastError().text();
        sqlitedb->rollback();
        return;
    }
    for (int i = 0; i < columnInfo.size(); ++i) {
        query.addBindValue(i < pieces.size() ? pieces[i] : QString());
    }
    if (query.exec()) {
        qInfo() << "Data inserted successfully!";
        sqlitedb->commit();
    } else {
        qCritical() << "Error inserting data:" << query.lastError().text();
        sqlitedb->rollback();
    }
}

void DB_handler::update_sensor_data(const wint_t& sensorId, QList<QList<QString>> data){
    data_of_sensors[sensorId] = data;

}