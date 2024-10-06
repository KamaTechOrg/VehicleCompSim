//
// Created by OWNER on 19/09/2024.
//

#include "initializeSensorsData.h"
#include <qstandardpaths.h>

void initializeSensorsData::initialize(){
    read_from_json();
    getSensorsInfoData();
}

void initializeSensorsData::read_from_json() {
    QString filePath = R"(C:\mobileye_project\VehicleCompSim\GUI\resources\jsons\box_info.json)";
//    QString dataDir;
//    QString relativeFilePath = "resources/jsons/box_info.json";

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

void initializeSensorsData::getSensorsInfoData() {
    QMap<int, QList<QList<QString>>> parseInfoMap;
    for (const auto &value: itemsArray) {
        QJsonObject obj = value.toObject();
        int id = obj["id"].toInt();
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
        parseInfoMap[id] = itemData;
    }
    GlobalState::getInstance().ParserInfo(parseInfoMap);
}