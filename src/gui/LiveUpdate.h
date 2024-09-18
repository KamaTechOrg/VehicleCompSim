//
// Created by OWNER on 07/08/2024.
//

#ifndef VEHICLECOMPSIM_LIVEUPDATE_H
#define VEHICLECOMPSIM_LIVEUPDATE_H


#include <QString>
#include <QSet>
#include <tuple>
#include <QRandomGenerator>
#include "customscene.h"

class LiveUpdate : public QObject {
Q_OBJECT
public:
    LiveUpdate(CustomScene* scene, QObject *parent = nullptr);

public:
    void parse_new_data(const QMap<QString, QVariantList> &last_changes);
    void change_view(const QString &srcId, const QString &destId);

private:
    CustomScene* m_scene = nullptr;
    QList<QColor> colorList = {
            QColor(255, 0, 255),   // Purple
            QColor(0, 0, 255),     // Blue
            QColor(255, 0, 0),     // Red
            QColor(0, 255, 255),   // Cyan
            QColor(0, 255, 0),     // Green
            QColor(255, 255, 0),   // Yellow
            QColor(255, 165, 0),   // Orange
            QColor(128, 0, 128),   // Dark Purple
            QColor(0, 128, 0),     // Dark Green
            QColor(0, 0, 0),       // Black
            QColor(255, 255, 255)  // White
    };
};



#endif //VEHICLECOMPSIM_LIVEUPDATE_H
