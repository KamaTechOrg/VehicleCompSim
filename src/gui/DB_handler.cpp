//
// Created by OWNER on 09/09/2024.
//

#include <bitset>
#include "DB_handler.h"

DB_handler::DB_handler() {
    sqlitedb = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    sqlitedb->setDatabaseName(R"(C:\Users\OWNER\Downloads\loginDetails.db)");

    // for test only
    QList<QString> data1 = {"A", "1", "wint_t"};
    QList<QString> data2 = {"B", "128", "QString"};
    QList<QString> data3 = {"C", "1", "wint_t"};
    QList<QList<QString>> new_sensor_data;
    new_sensor_data.emplace_back(data1);
    new_sensor_data.emplace_back(data2);
    new_sensor_data.emplace_back(data3);
    wint_t id = 5; // 7-bit unsigned integer
    data_of_sensors.insert(id ,new_sensor_data);
    // end test
}

QList<QVariant> parseBuffer(const QByteArray& buffer, const QList<QList<QString>>& columnInfo) {
    QList<QVariant> parsedValues;
    int offset = 0;
    for (const QList<QString>& column : columnInfo) {
        int bitLength = column[1].toInt();
        if(column[2] == "wint_t"){
            wint_t exit_id = buffer[offset] & 0x7F;
            qInfo() << exit_id;
            parsedValues.append(QVariant(exit_id));
        }else if(column[2] == "QString"){
            QString exit_message = QString::fromLatin1(buffer.mid(offset, offset + bitLength).constData());
            qInfo() << exit_message;
            parsedValues.append(QVariant(exit_message));
        }
        offset += bitLength;
    }
    return parsedValues;
}

void DB_handler::write_to_DB(const QByteArray& buffer) const {
    wint_t table_name = buffer[0] & 0x7F;
    if (sqlitedb->open()) {
        QSqlQuery query(*sqlitedb);
        QList<QList<QString>> columnInfo;
        if (data_of_sensors.contains(table_name)) {
            columnInfo = data_of_sensors.value(table_name);
        } else {
            columnInfo = data_of_sensors.value(00);
        }
//        QList<QList<QString>> columnInfo = data_of_sensors.value(table_name);
        QList<QVariant> parsed = parseBuffer(buffer, columnInfo);
        // Create the table if it doesn't exist
        QString createTableQuery = "CREATE TABLE IF NOT EXISTS '" + QString::number(table_name) + "' (";
        for (const QList<QString>& column : columnInfo) {
            createTableQuery += column[0] + " " + "TEXT" + ",";
        }
        createTableQuery.chop(1);
        createTableQuery += ")";
        qInfo() << createTableQuery;

        if (!query.exec(createTableQuery)) {
            qCritical() << "Failed to create table:" << query.lastError().text();
            sqlitedb->close();
            return;
        }
        // Construct the SQL query dynamically
        QString insertQuery = "INSERT INTO '" + QString::number(table_name) + "' (";
        QString valuePlaceholders = "(";
        QList<QString> names_for_bind;
        for (const QList<QString>& column : columnInfo) {
            insertQuery += column[0] + ",";
            valuePlaceholders += ":" + column[0] + ",";
            QString new_bind = ":" + column[0];
            names_for_bind.emplace_back(new_bind);
        }
        insertQuery.chop(1);
        valuePlaceholders.chop(1);
        insertQuery += ") VALUES " + valuePlaceholders + ")";
        query.prepare(insertQuery);
        int counter = 0;
        while(counter < names_for_bind.size()){
            query.bindValue(names_for_bind[counter], parsed[counter]);
            counter++;
        }
        if (query.exec()) {
            qInfo() << "Data inserted successfully!";
        } else {
            qInfo() << "Error inserting data:" << query.lastError().text();
        }
        sqlitedb->close();
    } else {
        qInfo() << "Database connection failed:" << sqlitedb->lastError().text();
    }
}

QList<QVariant> DB_handler::read_last_from_DB(const QString& table_name) {
    QList<QVariant> result;
    QString update_table_name = "'" + table_name + "'";
    if (sqlitedb->open()) {
        QSqlQuery query(*sqlitedb);
        QString selectQuery = QString("SELECT * FROM %1 ORDER BY ROWID DESC LIMIT 1").arg(update_table_name);
        if (query.exec(selectQuery)) {
            if (query.next()) {
                const QSqlRecord record = query.record();
                for (int i = 0; i < record.count(); ++i) {
                    result.append(query.value(i));
                }
            } else {
                qInfo() << "No records found in" << table_name << "table.";
            }
        } else {
            qInfo() << "Error executing query:" << query.lastError().text();
        }
        sqlitedb->close();
    } else {
        qInfo() << "Database connection failed:" << sqlitedb->lastError().text();
    }
    return result;
}