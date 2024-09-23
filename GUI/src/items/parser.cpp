//
// Created by OWNER on 23/09/2024.
//

#include "parser.h"

#include <utility>
#include "state/globalstate.h"

parser::parser(){
    connect(&GlobalState::getInstance(), &GlobalState::newDataArrived, this, &parser::parseBuffer);
    connect(&GlobalState::getInstance(), &GlobalState::ParserInfoArrived, this, &parser::setSensorInfoMap);
}

void parser::parseBuffer(const QByteArray& new_buffer) {
    qInfo() << "parseBuffer";
    QList<QPair<QString, QString>> result;
    QList<QString> defaultInfo = {"time", "src_id", "dest_id", "len"};
    QList<QByteArray> pieces = new_buffer.split(',');
    int counter = 0;
    while (counter < defaultInfo.size()) {
        QPair<QString, QString> new_pair = {};
        new_pair.first = defaultInfo[counter];
        new_pair.second = pieces[counter];
        result.append(new_pair);
        counter++;
    }
    QList<QList<QString>> columnInfo = sensorInfoMap[1];
    QByteArray data = pieces[4];
    int offset = 0;
    for (const QList<QString>& column : columnInfo) {
        const QString& name = column[0];
        int length = column[1].toInt();
        const QString& type = column[2];
        QPair<QString, QString> new_pair = {};
        new_pair.first = name;
        if (type == "char") {
            QByteArray extractedBytes = data.mid(offset, length);
            QString message = QString::fromLatin1(extractedBytes);
            new_pair.second = message;
        } else if (type == "int") {
            ///////
            int message = 0;
            QDataStream stream(data.mid(offset, length));
            stream.setByteOrder(QDataStream::LittleEndian);  // Adjust if necessary
            stream >> message;
//            qDebug() << "result int value:" << message;
            ///////
//            int message = data.mid(offset, length).toInt();
            new_pair.second = QString::number(message);
        } else if (type == "double") {
            double message = 0;
            QDataStream doubleReadStream(data.mid(offset, length));
            doubleReadStream.setByteOrder(QDataStream::LittleEndian); // Adjust if necessary
            doubleReadStream >> message;
//            qDebug() << "Result double value:" << message;
//            double message = data.mid(offset, length).toDouble();
            new_pair.second = QString::number(message);
        }
        result.append(new_pair);
        offset += length;
    }
    // Assuming 'result' is already populated with some data
//    for (const auto& pair : result) {
//        qInfo() << "Key:" << pair.first << " | Value:" << pair.second;
//    }
    GlobalState::getInstance().newParsedData(result);
}

void parser::setSensorInfoMap(QMap<int, QList<QList<QString>>> parseInfoMap) {
    qInfo() << "setSensorInfoMap";
    sensorInfoMap = std::move(parseInfoMap);
}
