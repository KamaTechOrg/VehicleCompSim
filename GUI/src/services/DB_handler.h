//
// Created by OWNER on 09/09/2024.
//

#ifndef VEHICLECOMPSIM_DB_HANDLER_H
#define VEHICLECOMPSIM_DB_HANDLER_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include "globalstate.h"
#include "../../../Communication/utilities/constants.h"
#include <QObject>




class DB_handler :public QObject{
Q_OBJECT
public:
    DB_handler();
public:
    QSqlDatabase *sqlitedb;
    QMap<wint_t, QList<QList<QString>>> data_of_sensors;
    QString dbFilePath;


public slots:
    void write_data_to_DB(const QString& data) const;
    void update_sensor_data(const wint_t& sensorId, QList<QList<QString>> data);
};


#endif //VEHICLECOMPSIM_DB_HANDLER_H
