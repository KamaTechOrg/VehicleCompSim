//
// Created by OWNER on 19/09/2024.
//

#ifndef VEHICLECOMPSIM_INITIALIZESENSORSDATA_H
#define VEHICLECOMPSIM_INITIALIZESENSORSDATA_H

#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <memory>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>
#include <QLineEdit>
#include <QGraphicsView>
#include <QToolBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QWidget>
#include <QPushButton>
#include <QRandomGenerator>
#include <QRect>
#include <QHBoxLayout>
#include <QJsonObject>
#include "globalstate.h"

class initializeSensorsData {

public:
    void initialize();

private:
    void getSensorsInfoData();
    void read_from_json();
    QJsonArray itemsArray;
};


#endif //VEHICLECOMPSIM_INITIALIZESENSORSDATA_H
