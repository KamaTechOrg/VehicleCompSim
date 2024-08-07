//
// Created by OWNER on 07/08/2024.
//

#ifndef VEHICLECOMPSIM_LIVEUPDATE_H
#define VEHICLECOMPSIM_LIVEUPDATE_H


#include <QString>
#include "customscene.h"

class LiveUpdate : public QObject {
Q_OBJECT
public:
    LiveUpdate(CustomScene* scene, QObject *parent = nullptr);

public:
    void parse_new_line(QString &line);
    void change_view();
private:
    CustomScene* m_scene;
    SensorItem *src_item;
    SensorItem *dest_item;
    int num_of_packets;
};



#endif //VEHICLECOMPSIM_LIVEUPDATE_H
