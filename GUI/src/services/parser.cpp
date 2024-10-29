//
// Created by OWNER on 23/09/2024.
//

#include "parser.h"

#include <utility>
#include "state/globalstate.h"
#include "state/globalconstants.h"
#include <QString>
#include <QList>
#include <QPair>
#include <iostream>
#include <QByteArray>
using namespace globalConstants;


parser::parser(){
    connect(&GlobalState::getInstance(), &GlobalState::newDataArrived, this, &parser::parseBuffer);
    connect(&GlobalState::getInstance(), &GlobalState::newRecordDataArrived, this, &parser::parseBuffer);
    connect(&GlobalState::getInstance(), &GlobalState::ParserInfoArrived, this, &parser::setSensorInfoMap);
}

void parser::setSensorInfoMap(QMap<int, QList<QList<QString>>> parseInfoMap) {
    sensorInfoMap = std::move(parseInfoMap);
}

void parser::parseBuffer(const QString& buffer) {
    QList<QPair<QString, QString>> result;
    QStringList defaultInfo = {"Time", "Src_id", "Dest_id", "Len"};
    QStringList split_pieces = buffer.split(',', Qt::SkipEmptyParts);
    for (int i = 0; i < defaultInfo.size() && i < split_pieces.size(); ++i) {
        result.append(qMakePair(defaultInfo[i], split_pieces[i]));
    }
    if(split_pieces.size() > 4){
        QString hexData = split_pieces[Buffer];
        QByteArray byteArray = QByteArray::fromHex(hexData.toLatin1());
        int src_id = split_pieces[SourceId].toInt();
        int buffer_size = split_pieces[Length].toInt();
        if (sensorInfoMap.contains(src_id)) {
            QList<QPair<QString, QString>> buffer_res = extractBufferData(byteArray, buffer_size, sensorInfoMap[src_id]);
            result.append(buffer_res);
//            for (const auto& pair : result) {
//                qDebug() << pair.first << ":" << pair.second;
//            }
            GlobalState::getInstance().newParsedData(result);
        } else {
                qWarning() << "Sensor info doesn't exist for src_id:" << src_id;
        }
    } else {
        qWarning() << "Buffer message doesn't exist in the data:" << buffer;
    }
}

QList<QPair<QString, QString>> parser::extractBufferData(const QByteArray& buffer, size_t bufferSize, const QList<QList<QString>>& columnInfo)
{
    QList<QPair<QString, QString>> result;
    size_t offset = 0;
    for (const QList<QString>& column : columnInfo) {
        if (column.size() < 3) {
            qWarning() << "Invalid column info:" << column;
            continue;
        }
        const QString& name = column[0];
        int length = column[1].toInt();
        const QString& type = column[2];
        QPair<QString, QString> new_pair(name, QString());
        if (offset >= bufferSize) {
            qWarning() << "Reached end of buffer prematurely for column:" << name;
            break;
        }

        if (type == "int32_t") {
            if (offset + sizeof(qint32) <= bufferSize) {
                int32_t value;
                memcpy(&value, buffer.data() + offset, sizeof(int32_t));
                offset += sizeof(int32_t);
                value = qFromLittleEndian(value);
//                value = qFromBigEndian(value);
                new_pair.second = QString::number(value);
            }
        }
        else if (type == "uint32_t") {
            if (offset + sizeof(quint32) <= bufferSize) {
                uint32_t value;
                memcpy(&value, buffer.data() + offset, sizeof(uint32_t));
                value = qFromLittleEndian(value);
//                value = qFromBigEndian(value);
                new_pair.second = QString::number(value);
                offset += sizeof(quint32);
            }
        }
        else if (type == "char*") {
            if (offset + length <= bufferSize) {
                char msg[length - 1];
                memcpy(msg, buffer.data() + offset, length - 1);
                new_pair.second = QString::fromUtf8(msg);
                offset += length;
            }
        }
        else if (type == "float") {
            if (offset + sizeof(float) <= bufferSize) {
                float value;
                memcpy(&value, buffer.data() + offset, sizeof(float));
                offset += sizeof(float);
                new_pair.second = QString::number(value, 'g', 6);
            }
        }
        else if (type == "float mantissa") {
            if (offset + sizeof(float) + sizeof(int32_t) <= bufferSize) {
                float value;
                int32_t mantissa;
                memcpy(&value, buffer.data() + offset, sizeof(float));
                offset += sizeof(float);
                memcpy(&mantissa, buffer.data() + offset, sizeof(int32_t));
                offset += sizeof(int32_t);
                mantissa = qFromLittleEndian(mantissa);
                double fullValue = value * std::pow(10, mantissa);
                new_pair.second = QString::number(fullValue, 'g', 15);
            }
        }
        else if (type == "double") {
            if (offset + sizeof(double) <= bufferSize) {
                double value;
                memcpy(&value, buffer.data() + offset, sizeof(double));
                new_pair.second = QString::number(value, 'g', 15);
                offset += sizeof(double);
            }
        }
        else if (type == "bool") {
            uint8_t flagsByte;
            memcpy(&flagsByte, buffer.data() + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            QStringList boolResults;
            for (int i = 0; i < length; i++) {
                bool bitValue = (flagsByte >> i) & 0x01;
                boolResults.append(bitValue ? "true" : "false");
            }
            new_pair.second = boolResults.join(" ");
        }
        else {
            qWarning() << "Unsupported type:" << type << "for column:" << name;
            new_pair.second = "Unsupported type";
        }

        result.append(new_pair);
    }

    return result;
}
