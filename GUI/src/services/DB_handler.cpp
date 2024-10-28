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
    initializeDB();
    flushTimer = new QTimer(this);
    connect(flushTimer, &QTimer::timeout, this, &DB_handler::flushBuffer);
    flushTimer->start(5000);
    connect(&GlobalState::getInstance(), &GlobalState::newDataArrived,
            this, &DB_handler::buffer_data);
}
DB_handler::~DB_handler() {
    flushBuffer();
    delete flushTimer;
    delete sqlitedb;
}

void DB_handler::buffer_data(const QString& data) {
    dataBuffer.append(data);
    if (dataBuffer.size() >= BATCH_SIZE) {
        flushBuffer();
    }
}

void DB_handler::initializeDB() {
    const QStringList columnInfo = {"time", "src_id", "dest_id", "len", "buffer"};
    if (!sqlitedb->isOpen() && !sqlitedb->open()) {
        qCritical() << "Database connection failed:" << sqlitedb->lastError().text();
        return;
    }

    QSqlQuery query(*sqlitedb);
    const QString createTableQuery = QString(
            "CREATE TABLE IF NOT EXISTS DB (%1)"
    ).arg(columnInfo.join(" TEXT, ") + " TEXT");

    if (!query.exec(createTableQuery)) {
        qCritical() << "Failed to create table:" << query.lastError().text();
        return;
    }
    // Enable WAL mode for better concurrent access
    query.exec("PRAGMA journal_mode = WAL");
    // Disable synchronous writes for better performance
    query.exec("PRAGMA synchronous = NORMAL");
    // Increase cache size
    query.exec("PRAGMA cache_size = -2000"); // 2MB cache
}

void DB_handler::flushBuffer() {
    if (dataBuffer.isEmpty()) return;

    const QStringList columnInfo = {"time", "src_id", "dest_id", "len", "buffer"};

    if (!sqlitedb->isOpen() && !sqlitedb->open()) {
        qCritical() << "Database connection failed:" << sqlitedb->lastError().text();
        return;
    }
    QSqlQuery query(*sqlitedb);
    sqlitedb->transaction();
    QString insertQuery = QString(
            "INSERT INTO DB (%1) VALUES (%2)"
    ).arg(columnInfo.join(", "), QStringList(columnInfo.size(), "?").join(", "));
    if (!query.prepare(insertQuery)) {
        qCritical() << "Failed to prepare insert query:" << query.lastError().text();
        sqlitedb->rollback();
        return;
    }
    for (const QString& data : dataBuffer) {
        QStringList pieces = data.split(',', Qt::SkipEmptyParts);
        for (int i = 0; i < columnInfo.size(); ++i) {
            query.addBindValue(i < pieces.size() ? pieces[i] : QString());
        }

        if (!query.exec()) {
            qCritical() << "Error inserting data:" << query.lastError().text();
            sqlitedb->rollback();
            return;
        }
    }

    if (sqlitedb->commit()) {
        dataBuffer.clear();
    } else {
        qCritical() << "Failed to commit transaction:" << sqlitedb->lastError().text();
        sqlitedb->rollback();
    }
}

void DB_handler::update_sensor_data(const wint_t& sensorId, QList<QList<QString>> data){
    data_of_sensors[sensorId] = data;

}