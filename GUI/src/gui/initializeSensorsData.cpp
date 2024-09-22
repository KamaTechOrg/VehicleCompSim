//
// Created by OWNER on 19/09/2024.
//

#include "initializeSensorsData.h"

void initializeSensorsData::initialize(){
    read_from_json();
    fill_db_data();
    fill_box_data();
}

void initializeSensorsData::fill_box_data() {
    for (auto value: itemsArray) {
        QJsonObject obj = value.toObject();
        int id = obj["id"].toInt(); // Assuming 'id' is an integer
        QString idString = QString::number(id); // Convert the integer ID to a QString
        QJsonArray dataArray = obj["data"].toArray();
        QList<QString> namesList;
        for (auto dataValue: dataArray) {
            QJsonObject dataObj = dataValue.toObject();
            namesList << dataObj["name"].toString();
        }
        GlobalState::getInstance().updateColumnNames(idString, namesList);
    }
}
void initializeSensorsData::read_from_json() {
    const QString& filePath = R"(C:\mobileye_project\VehicleCompSim\GUI\src\gui\box_info.json)";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file" << filePath << ": " << file.errorString();
        return;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &parseError);
    if (document.isNull()) {
        qWarning() << "Failed to create JSON doc.";
        qWarning() << "Error:" << parseError.errorString();
        qWarning() << "Offset:" << parseError.offset;
        return;
    }
    if (!document.isObject()) {
        qWarning() << "JSON is not an object.";
        return;
    }
    QJsonObject jsonObj = document.object();
    itemsArray = jsonObj["items"].toArray();
    qInfo() << "Successfully read from JSON. Number of items:" << itemsArray.size();
}

void initializeSensorsData::fill_db_data() {
    for (const auto &value: itemsArray) {
        QJsonObject obj = value.toObject();
        wint_t id = obj["id"].toInt();
//        qInfo() << "wint_t id" << id;
        QJsonArray dataArray = obj["data"].toArray();

        QList<QList<QString>> itemData;
        for (const auto &dataValue: dataArray) {
            QJsonObject dataObj = dataValue.toObject();
            QList<QString> row;
            row << dataObj["name"].toString()
                << dataObj["numBits"].toString()
                << dataObj["type"].toString();
            itemData.append(row);
        }
        GlobalState::getInstance().updateDbHandler(id, itemData);
    }
}