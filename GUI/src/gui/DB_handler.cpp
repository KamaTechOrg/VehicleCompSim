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
    connect(&GlobalState::getInstance(), &GlobalState::newDataArrived, this, &DB_handler::write_data_to_DB, Qt::QueuedConnection);
}
void DB_handler::write_data_to_DB(const QString& data) const {
    const QStringList columnInfo = {"time", "src_id", "dest_id", "len", "buffer"};
    const QStringList pieces = data.split(',');

    if (!sqlitedb->isOpen() && !sqlitedb->open()) {
        qCritical() << "Database connection failed:" << sqlitedb->lastError().text();
        return;
    }

    QSqlQuery query(*sqlitedb);
    sqlitedb->transaction();

    // Create table if not exists
    const QString createTableQuery = QString(
            "CREATE TABLE IF NOT EXISTS DB (%1)"
    ).arg(columnInfo.join(" TEXT, ") + " TEXT");

    if (!query.exec(createTableQuery)) {
        qCritical() << "Failed to create table:" << query.lastError().text();
        sqlitedb->rollback();
        return;
    }

    // Prepare insert query
    const QString insertQuery = QString(
            "INSERT INTO DB (%1) VALUES (%2)"
    ).arg(columnInfo.join(", "), QStringList(columnInfo.size(), "?").join(", "));

    if (!query.prepare(insertQuery)) {
        qCritical() << "Failed to prepare insert query:" << query.lastError().text();
        sqlitedb->rollback();
        return;
    }

    // Bind values
    for (int i = 0; i < columnInfo.size(); ++i) {
        query.addBindValue(i < pieces.size() ? pieces[i] : QString());
    }

    // Execute insert query
    if (query.exec()) {
        qInfo() << "Data inserted successfully!";
        sqlitedb->commit();
    } else {
        qCritical() << "Error inserting data:" << query.lastError().text();
        sqlitedb->rollback();
    }
}


//QList<QVariant> parseBuffer(const QByteArray& buffer, const QList<QList<QString>>& columnInfo) {
//    QList<QVariant> parsedValues;
//    int offset = 0;
//    for (const QList<QString>& column : columnInfo) {
//        if(column == columnInfo[columnInfo.size()-1]){
////            qInfo() << "QString";
//            QString time_stamp = QString::fromLatin1(buffer.mid(200 - 24, 200).constData());
////            qInfo() << time_stamp;
//            parsedValues.append(QVariant(time_stamp));
//            break;
//        }
//        int bitLength = column[1].toInt();
//        if(column[2] == "wint_t"){
////            qInfo() << "wint_t";
//            wint_t exit_id = buffer[offset] & 0x7F;
//            parsedValues.append(QVariant(exit_id));
//        }else if(column[2] == "QString"){
//            QString exit_message2 = QString::fromLatin1(buffer.mid(offset, offset + bitLength).constData());
//            parsedValues.append(QVariant(exit_message2));
//        }
//        offset += bitLength;
//    }
//    return parsedValues;
//}

//void DB_handler::write_to_DB(const QByteArray& buffer) const {
//    QList<QByteArray> pieces = buffer.split(',');
//    int src_id = pieces.value(1).toInt();
//    if (sqlitedb->open()) {
//        QSqlQuery query(*sqlitedb);
//        QList<QList<QString>> columnInfo;
//        if (data_of_sensors.contains(src_id)) {
//            columnInfo = data_of_sensors.value(src_id);
//        } else {
//            columnInfo = data_of_sensors.value(0);
//        }
//        QList<QVariant> parsed = parseBuffer(pieces[4], columnInfo);
//        QString createTableQuery = "CREATE TABLE IF NOT EXISTS '" + QString::number(src_id) + "' (";
//        for (const QList<QString>& column : columnInfo) {
//            createTableQuery += column[0] + " " + "TEXT" + ",";
//        }
//        createTableQuery.chop(1);
//        createTableQuery += ")";
////        qInfo() << createTableQuery << createTableQuery;
//        if (!query.exec(createTableQuery)) {
//            qCritical() << "Failed to create table:" << query.lastError().text();
//            sqlitedb->close();
//            return;
//        }
//        QString insertQuery = "INSERT INTO '" + QString::number(src_id) + "' (";
//        QString valuePlaceholders = "(";
//        QList<QString> names_for_bind;
//        for (const QList<QString>& column : columnInfo) {
//            insertQuery += column[0] + ",";
//            valuePlaceholders += ":" + column[0] + ",";
//            QString new_bind = ":" + column[0];
//            names_for_bind.emplace_back(new_bind);
//        }
//        insertQuery.chop(1);
//        valuePlaceholders.chop(1);
//        insertQuery += ") VALUES " + valuePlaceholders + ")";
//        query.prepare(insertQuery);
////        for(const auto &item : names_for_bind){
////            qInfo () << item;
////        }
//        int counter = 0;
//        while(counter < names_for_bind.size() - 1){
//            if(counter < 2){
//                query.bindValue(names_for_bind[counter], pieces[counter + 1]);
//            }else{
//                query.bindValue(names_for_bind[counter], parsed[counter - 2]);
//            }
//            counter++;
//        }
//        query.bindValue(names_for_bind[counter], pieces[0]);
//        if (query.exec()) {
//            qInfo() << "Data inserted successfully!";
//        } else {
//            qInfo() << "Error inserting data:" << query.lastError().text();
//        }
//        sqlitedb->close();
//    } else {
//        qInfo() << "Database connection failed:" << sqlitedb->lastError().text();
//    }
//}
//QList<QVariant> DB_handler::read_all_sensor_data(const QString& table_name) {
//    QList<QVariant> result;
//    QString update_table_name = "'" + table_name + "'";
//    if (sqlitedb->open()) {
//        QSqlQuery query(*sqlitedb);
//        QString selectQuery = QString("SELECT * FROM %1 ORDER BY ROWID DESC").arg(update_table_name);
//        if (query.exec(selectQuery)) {
//            while (query.next()) {
//                const QSqlRecord record = query.record();
//                for (int i = 0; i < record.count(); ++i) {
//                    result.append(query.value(i));
//                }
//            }
//        } else {
//            qInfo() << "Error executing query:" << query.lastError().text();
//        }
//        sqlitedb->close();
//    } else {
//        qInfo() << "Database connection failed:" << sqlitedb->lastError().text();
//    }
//    return result;
//}

void DB_handler::update_sensor_data(const wint_t& sensorId, QList<QList<QString>> data){
    data_of_sensors[sensorId] = data;

}



//QList<QVariant> DB_handler::read_last_from_DB(const QString& table_name) {
//    QList<QVariant> result;
//    QString update_table_name = "'" + table_name + "'";
//    if (sqlitedb->open()) {
//        QSqlQuery query(*sqlitedb);
//        QString selectQuery = QString("SELECT * FROM %1 ORDER BY ROWID DESC LIMIT 1").arg(update_table_name);
//        if (query.exec(selectQuery)) {
//            if (query.next()) {
//                const QSqlRecord record = query.record();
//                for (int i = 0; i < record.count(); ++i) {
//                    result.append(query.value(i));
//                }
//            } else {
//                qInfo() << "No records found in" << table_name << "table.";
//            }
//        } else {
//            qInfo() << "Error executing query:" << query.lastError().text();
//        }
//        sqlitedb->close();
//    } else {
//        qInfo() << "Database connection failed:" << sqlitedb->lastError().text();
//    }
//    return result;
//}

