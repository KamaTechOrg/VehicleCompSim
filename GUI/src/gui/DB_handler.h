//
// Created by OWNER on 09/09/2024.
//

#ifndef VEHICLECOMPSIM_DB_HANDLER_H
#define VEHICLECOMPSIM_DB_HANDLER_H

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QGraphicsView>
#include "globalstate.h"



class DB_handler :public QObject{
Q_OBJECT
public:
    DB_handler();
    void write_to_DB(const QByteArray& buffer) const;
//    QList<QVariant> read_last_from_DB(const QString& table_name);
    QList<QVariant> read_all_sensor_data(const QString& table_name);
//    void update_sensor_data(const QString& sensorId, QList<QList<QString>> data);
public:
    QSqlDatabase *sqlitedb;
    QMap<wint_t, QList<QList<QString>>> data_of_sensors;


public slots:
    void update_sensor_data(const wint_t& sensorId, QList<QList<QString>> data);
};


#endif //VEHICLECOMPSIM_DB_HANDLER_H
