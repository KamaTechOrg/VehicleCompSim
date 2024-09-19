//
// Created by OWNER on 07/08/2024.
//

#include "LiveUpdate.h"
LiveUpdate::LiveUpdate(CustomScene* scene, QObject *parent)
        : m_scene(scene) {}

void LiveUpdate::parse_new_data(const QMap<QString, QVariantList> &last_changes) {
//    qInfo() << "parse_new_data";
    QSet<QString> itemIds;

    // Sort the entries by timestamp in descending order
    QList<QPair<QDateTime, QPair<QString, QString>>> sortedEntries;

    for (const auto &entry : last_changes) {
        const QVariantList &data = entry;
        if (data.size() >= 3) {
            QDateTime timestamp = data.last().toDateTime();
            QString srcId = data[0].toString();
            QString destId = data[1].toString();
            sortedEntries.append({timestamp, {srcId, destId}});
        }
    }

    std::sort(sortedEntries.begin(), sortedEntries.end(),
              [](const auto &a, const auto &b) { return a.first > b.first; });

    // Iterate through sorted entries
    for (const auto &entry : sortedEntries) {
        const QString &srcId = entry.second.first;
        const QString &destId = entry.second.second;

        // Check if both srcId and destId have not been processed before
        if (!itemIds.contains(srcId) && !itemIds.contains(destId)) {
//            qInfo() << "need to change_view";
            change_view(srcId, destId);
            itemIds.insert(srcId);
            itemIds.insert(destId);
        }
    }
}

void LiveUpdate::change_view(const QString &srcId, const QString &destId) {
//    qInfo() << "change_view";
    SensorItem *src_item = nullptr;
    SensorItem *dest_item = nullptr;
    if(m_scene == nullptr){
//        qInfo() << "null";
        return;
    }
//    for (auto item: m_scene->items()) {
//        if (auto *sensor = dynamic_cast<SensorItem *>(item)) {
//            QString sensorId = sensor->getPriority();;
////            qInfo() << "sensorId" << sensorId;
//            if (sensorId == srcId || sensorId == destId) {
//                if (sensorId == srcId && src_item == nullptr) {
//                    src_item = sensor;
////                    qInfo() << "set src_item to srcId";
//                } else if (sensorId == srcId && dest_item == nullptr) {
//                    dest_item = sensor;
////                    qInfo() << "set dest_item to srcId";
//                }
//                if (sensorId == destId && src_item == nullptr) {
//                    src_item = sensor;
////                    qInfo() << "set src_item to destId";
//
//                } else if (sensorId == destId && dest_item == nullptr) {
//                    dest_item = sensor;
////                    qInfo() << "set dest_item to destId";
//
//                }
//            }
//        }
//    }
//    qInfo() << "SET SRC DEST";
    // Generate a random index
    int randomIndex = QRandomGenerator::global()->bounded(colorList.size());

    // Get the random color
    QColor randomColor = colorList[randomIndex];

    // Extract r, g, b values
    int r = randomColor.red();
    int g = randomColor.green();
    int b = randomColor.blue();

    // Assuming src_item and dest_item are valid pointers
    if (src_item) {
        src_item->set_m_color(r, g, b);
        src_item->update();
    }
    if (dest_item) {
        dest_item->set_m_color(r, g, b);
        dest_item->update();
    }
//    qInfo() << "Change completed";
}