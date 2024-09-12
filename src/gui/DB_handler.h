//
// Created by OWNER on 09/09/2024.
//

#ifndef VEHICLECOMPSIM_DB_HANDLER_H
#define VEHICLECOMPSIM_DB_HANDLER_H

#include <QtSql>
#include <QSqlDatabase>


class DB_handler {

public:
    DB_handler();
    void write_to_DB(const QByteArray& buffer) const;
    QList<QVariant> read_last_from_DB(const QString& table_name);

public:
    QSqlDatabase *sqlitedb;
    QMap<wint_t, QList<QList<QString>>> data_of_sensors;

};


#endif //VEHICLECOMPSIM_DB_HANDLER_H
