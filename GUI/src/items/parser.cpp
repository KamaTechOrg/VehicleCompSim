//
// Created by OWNER on 23/09/2024.
//

#include "parser.h"

#include <utility>
#include "state/globalstate.h"
#include <QString>
#include <QList>
#include <QPair>
#include <QStringList>

parser::parser(){
    connect(&GlobalState::getInstance(), &GlobalState::newDataArrived, this, &parser::parseBuffer, Qt::QueuedConnection);
    connect(&GlobalState::getInstance(), &GlobalState::ParserInfoArrived, this, &parser::setSensorInfoMap);
}

void parser::setSensorInfoMap(QMap<int, QList<QList<QString>>> parseInfoMap) {
//    qInfo() << "setSensorInfoMap";
    sensorInfoMap = std::move(parseInfoMap);
}

void parser::parseBuffer(const QString& data) {
    QList<QPair<QString, QString>> result = {};
    QStringList defaultInfo = {"time", "src_id", "dest_id", "len"};
    QStringList split_pieces = data.split(',');
    // Process default information
    for (int i = 0; i < defaultInfo.size() && i < split_pieces.size(); ++i) {
        result.append(qMakePair(defaultInfo[i], split_pieces[i]));
    }
    if (split_pieces.size() > 4) {
        // Deserialize buffer
        const char *buffer = split_pieces[4].toLatin1();
        if (sensorInfoMap.contains(split_pieces[1].toInt())) {
            QList<QList<QString>> columnInfo = sensorInfoMap[split_pieces[1].toInt()];
            int exit_index = 0;
            for (const QList<QString>& column : columnInfo) {
                if (column.size() < 3) continue;  // Skip if column info is incomplete

                const QString &name = column[0];
                int length = column[1].toInt();
                const QString &type = column[2];

                QPair<QString, QString> new_pair;
                new_pair.first = name;
                if (type == "string") {
                    new_pair.second = QString::fromLatin1(buffer + exit_index, length);
//                exit_index += length;
                } else if (type == "double") {
                    double value;
                    memcpy(&value, buffer + exit_index, sizeof(double));
                    new_pair.second = QString::number(value);
//                    exit_index += sizeof(double);
                } else if (type == "int") {
                    int value;
                    memcpy(&value, buffer + exit_index, sizeof(int));
                    new_pair.second = QString::number(value);
//                    exit_index += sizeof(int);
                }
                result.append(new_pair);
                exit_index += length;

            }
            // Use the result list as needed
            //    for (const auto& pair : result) {
            //        qDebug() << pair.first << ":" << pair.second;
            //    }
            GlobalState::getInstance().newParsedData(result);
        } else {
            qInfo() << "sensor info doesn't exist";
        }
    }
    qInfo() << "buffer message doesn't exist";
}

