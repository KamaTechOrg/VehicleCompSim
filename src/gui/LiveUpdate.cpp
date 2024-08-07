//
// Created by OWNER on 07/08/2024.
//

#include "LiveUpdate.h"
LiveUpdate::LiveUpdate(CustomScene* scene, QObject *parent)
        : m_scene(scene) {}

void LiveUpdate::parse_new_line(QString &line){
    QStringList communication_data = line.split(',');
    src_item = nullptr;
    dest_item = nullptr;
    if (communication_data.size() == 3) {
        QString src = communication_data[0];
        QString dest = communication_data[1];
        num_of_packets = communication_data[2].toInt();
        for (auto item: m_scene->items()) {
            if (auto *base = dynamic_cast<SensorItem *>(item)) {
                if (base->getID() == src) {
                    this->src_item = base;
                }
                if (base->getID() == dest) {
                    dest_item = base;
                }
            }
        }

        if(src_item != nullptr && dest_item != nullptr){
            change_view();
        }
    }
}
void LiveUpdate::change_view(){
    int r, g, b;
    if (num_of_packets == 0) {
        r = 255;
        g = 0;
        b = 255;
    }else if (num_of_packets > 0 && num_of_packets < 10) {
        r = 0;
        g = 0;
        b = 255;
    }else if (num_of_packets > 10 && num_of_packets < 20) {
        r = 255;
        g = 0;
        b = 0;
    }else if (num_of_packets > 20) {
        r = 0;
        g = 255;
        b = 255;
    }
    src_item->set_m_color(r, g, b);
    dest_item->set_m_color(r, g, b);
    src_item->update();
    dest_item->update();
}