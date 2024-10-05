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
    QString dataDir;
    QString relativeFilePath = "resources/jsons/box_info.json";

    // TOTO:
    // if (qEnvironmentVariableIsSet("DEV_MODE")) {
    //     dataDir = QDir::currentPath(); // Use current directory in development mode
    // } else {
    //     dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation); // Use AppDataLocation in production
    // }
    dataDir = QDir::currentPath();
    QString filePath = QDir::cleanPath(QDir(dataDir).filePath(relativeFilePath));
    
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