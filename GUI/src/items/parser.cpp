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
#include <iostream>
#include <cstdint>
#include <cstring> // For std::memcpy and std::strlen
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QList>
#include <QPair>


parser::parser(){
    connect(&GlobalState::getInstance(), &GlobalState::newDataArrived, this, &parser::parseBuffer, Qt::QueuedConnection);
    connect(&GlobalState::getInstance(), &GlobalState::ParserInfoArrived, this, &parser::setSensorInfoMap);
}

void parser::setSensorInfoMap(QMap<int, QList<QList<QString>>> parseInfoMap) {
//    qInfo() << "setSensorInfoMap";
    sensorInfoMap = std::move(parseInfoMap);
}
//void parser::testExtractBufferData() {
//    // Create a sample buffer with all supported types
//    QByteArray testBuffer;
//    QDataStream stream(&testBuffer, QIODevice::WriteOnly);
//    stream.setByteOrder(QDataStream::LittleEndian);
//
//    int time = 222;
//    int src_id = 1;
//    int dest_id = 2;
//    int len = 50;
//    stream << time << ',' << src_id << ',' << dest_id << ',' << len << ',';
//
//    // int32_t
//    qint32 testInt32 = -12345;
//    stream << testInt32;
//
//    // uint32_t
//    quint32 testUint32 = 54321;
//    stream << testUint32;
//
//    // char*
//    const char *testString = "Hello, World!";
//    stream.writeRawData(testString, 14); // Including null terminator
//
//    // float
//    float testFloat = 3.14159f;
//    stream.writeRawData(reinterpret_cast<const char *>(&testFloat), sizeof(float));
//
//    // double
//    double testDouble = 2.71828182845904;
//    stream.writeRawData(reinterpret_cast<const char *>(&testDouble), sizeof(double));
//
//    // bool (3 booleans)
//    quint8 testBools = 0b00000101; // true, false, true
//    stream << testBools;
//
//    QByteArray originalData; // Your original QByteArray
//    QString base64String = QString::fromLatin1(originalData.toBase64());
//    GlobalState::getInstance().new_test_buffer(base64String);
//}

//    // Create column info
//    QList<QList<QString>> columnInfo = {
//            {"TestInt32", "4", "int32_t"},
//            {"TestUint32", "4", "uint32_t"},
//            {"TestString", "14", "char*"},
//            {"TestFloat", "4", "float"},
//            {"TestDouble", "8", "double"},
//            {"TestBools", "3", "bool"}
//    };
//void parser::create_buffer() {
//    // Create a buffer
//    uint8_t buffer[100]; // Adjust the size as needed
//
//    // Add signed and unsigned integers
//    int32_t signedInt = 42;
//    uint32_t unsignedInt = 123;
//    std::memcpy(buffer, &signedInt, sizeof(signedInt));
//    std::memcpy(buffer + sizeof(signedInt), &unsignedInt, sizeof(unsignedInt));
//
//    // Add a character array
//    const char *charArray = "Hello, world!";
//    std::memcpy(buffer + sizeof(signedInt) + sizeof(unsignedInt), charArray, std::strlen(charArray) + 1);
//
//    // Add floating point values
//    float fixedPrecisionFloat = 3.14f;
//    double mantissaFloat = 123.456;
//    std::memcpy(buffer + sizeof(signedInt) + sizeof(unsignedInt) + std::strlen(charArray) + 1,
//                &fixedPrecisionFloat, sizeof(fixedPrecisionFloat));
//    std::memcpy(
//            buffer + sizeof(signedInt) + sizeof(unsignedInt) + std::strlen(charArray) + 1 + sizeof(fixedPrecisionFloat),
//            &mantissaFloat, sizeof(mantissaFloat));
//
//    // Add bit-fields (flags)
//    struct Flags {
//        bool flag1: 1;
//        bool flag2: 1;
//        // Add more flags as needed
//    };
//    Flags flags;
//    flags.flag1 = true;
//    flags.flag2 = false;
//    std::memcpy(buffer + sizeof(signedInt) + sizeof(unsignedInt) + std::strlen(charArray) + 1 +
//                sizeof(fixedPrecisionFloat) + sizeof(mantissaFloat),
//                &flags, sizeof(flags));
//
//    parseBuffer(buffer);
//    std::string
//
//
//    // Now you have your buffer with all the data types!
//}

void parser::parseBuffer(const char buffer[], size_t bufferSize) {
    QList<QPair<QString, QString>> result;
    QStringList defaultInfo = {"time", "src_id", "dest_id", "len"};
    qInfo() << "parseBuffer" << buffer[0];

    QString dataString = QString::fromLatin1(buffer);
    // Split the string by commas
    QStringList split_pieces = dataString.split(',', Qt::SkipEmptyParts);
    qInfo() << split_pieces.size();
    for(const auto& item : split_pieces){
        qInfo() << item;
    }
    for (int i = 0; i < defaultInfo.size() && i < split_pieces.size(); ++i) {
        result.append(qMakePair(defaultInfo[i], split_pieces[i]));
    }
    if (split_pieces.size() > 4) {
        ////////////////////////////////////////
//        QString firstItem = dataPieces.first().trimmed();
//        // Convert QString to char array
//        QByteArray firstItemBytes = firstItem.toLocal8Bit();
//        // Determine the size of the buffer
//        const int bufferSize = 256;  // Adjust this size as needed
//        char buffer[bufferSize];
//        // Copy the data to the buffer, ensuring we don't overflow
//        int copySize = qMin(bufferSize - 1, firstItemBytes.size());
//        memcpy(buffer, firstItemBytes.constData(), copySize);
//        ////////////////////////////////////////
//        QByteArray buffer = QByteArray::fromHex(split_pieces[4].toLatin1());
        int src_id = split_pieces[1].toInt();
        int buffer_size = split_pieces[3].toInt();
        if (sensorInfoMap.contains(src_id)) {
            QList<QPair<QString, QString>> buffer_res = extractBufferData(buffer, buffer_size, sensorInfoMap[src_id]);
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

// without stream
QList<QPair<QString, QString>> parser::extractBufferData(const char buffer[], size_t bufferSize, const QList<QList<QString>>& columnInfo)
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
                qint32 value;
                memcpy(&value, buffer + offset, sizeof(qint32));
                value = qFromLittleEndian(value);
                new_pair.second = QString::number(value);
                offset += sizeof(qint32);
            }
        }
        else if (type == "uint32_t") {
            if (offset + sizeof(quint32) <= bufferSize) {
                quint32 value;
                memcpy(&value, buffer + offset, sizeof(quint32));
                value = qFromLittleEndian(value);
                new_pair.second = QString::number(value);
                offset += sizeof(quint32);
            }
        }
        else if (type == "char*") {
            if (offset + length <= bufferSize) {
                new_pair.second = QString::fromUtf8(buffer + offset, length).trimmed();
                offset += length;
            }
        }
        else if (type == "float") {
            if (offset + sizeof(float) <= bufferSize) {
                float value;
                memcpy(&value, buffer + offset, sizeof(float));
                new_pair.second = QString::number(value, 'g', 6);
                offset += sizeof(float);
            }
        }
        else if (type == "double") {
            if (offset + sizeof(double) <= bufferSize) {
                double value;
                memcpy(&value, buffer + offset, sizeof(double));
                new_pair.second = QString::number(value, 'g', 15);
                offset += sizeof(double);
            }
        }
        else if (type == "bool") {
            QStringList boolResults;
            for (int i = 0; i < length && offset < bufferSize; i++) {
                quint8 flagsByte = static_cast<quint8>(buffer[offset]);
                boolResults.append((flagsByte & 0x01) ? "true" : "false");
                offset++;
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
//// with enndian
//QList<QPair<QString, QString>> extractBufferData(const QByteArray& buffer, const QList<QList<QString>>& columnInfo)
//{
//    QDataStream stream(buffer);
//    QList<QPair<QString, QString>> result;
//
//    for (const QList<QString>& column : columnInfo) {
//        if (column.size() < 4) {
//            qWarning() << "Invalid column info:" << column;
//            continue;
//        }
//        const QString& name = column[0];
//        int sizeBits = column[1].toInt();
//        const QString& type = column[2];
//        const QString& endianness = column[3].toLower(); // "big" or "little"
//
//        stream.setByteOrder(endianness == "big" ? QDataStream::BigEndian : QDataStream::LittleEndian);
//
//        QPair<QString, QString> new_pair(name, QString());
//        if (stream.atEnd()) {
//            qWarning() << "Reached end of stream prematurely for column:" << name;
//            break;
//        }
//
//        if (type.startsWith("int")) {
//            bool ok;
//            int bitSize = type.midRef(3).toInt(&ok);
//            if (!ok || bitSize > 64) {
//                qWarning() << "Invalid integer size for column:" << name;
//                continue;
//            }
//            qint64 value = 0;
//            stream.readRawData(reinterpret_cast<char*>(&value), bitSize / 8);
//            if (endianness == "big") {
//                value = qFromBigEndian(value);
//            }
//            value = (value << (64 - bitSize)) >> (64 - bitSize); // Sign extend if necessary
//            new_pair.second = QString::number(value);
//        }
//        else if (type.startsWith("uint")) {
//            bool ok;
//            int bitSize = type.midRef(4).toInt(&ok);
//            if (!ok || bitSize > 64) {
//                qWarning() << "Invalid unsigned integer size for column:" << name;
//                continue;
//            }
//            quint64 value = 0;
//            stream.readRawData(reinterpret_cast<char*>(&value), bitSize / 8);
//            if (endianness == "big") {
//                value = qFromBigEndian(value);
//            }
//            value &= (1ULL << bitSize) - 1; // Mask off any extra bits
//            new_pair.second = QString::number(value);
//        }
//        else if (type == "char*") {
//            QByteArray charArray(sizeBits / 8, '\0');
//            if (stream.readRawData(charArray.data(), sizeBits / 8) != sizeBits / 8) {
//                qWarning() << "Failed to read full char array for column:" << name;
//            }
//            new_pair.second = QString::fromUtf8(charArray).trimmed();
//        }
//        else if (type == "float") {
//            float value;
//            stream.readRawData(reinterpret_cast<char*>(&value), sizeof(float));
//            if (endianness == "big") {
//                union { float f; quint32 i; } u;
//                u.f = value;
//                u.i = qFromBigEndian(u.i);
//                value = u.f;
//            }
//            new_pair.second = QString::number(value, 'g', 6);
//        }
//        else if (type == "double") {
//            double value;
//            stream.readRawData(reinterpret_cast<char*>(&value), sizeof(double));
//            if (endianness == "big") {
//                union { double d; quint64 i; } u;
//                u.d = value;
//                u.i = qFromBigEndian(u.i);
//                value = u.d;
//            }
//            new_pair.second = QString::number(value, 'g', 15);
//        }
//        else if (type == "bitfield") {
//            quint64 value = 0;
//            stream.readRawData(reinterpret_cast<char*>(&value), (sizeBits + 7) / 8);
//            if (endianness == "big") {
//                value = qFromBigEndian(value);
//            }
//            value &= (1ULL << sizeBits) - 1; // Mask off any extra bits
//            new_pair.second = QString::number(value, 2).rightJustified(sizeBits, '0');
//        }
//        else {
//            qWarning() << "Unsupported type:" << type << "for column:" << name;
//            new_pair.second = "Unsupported type";
//        }
//        result.append(new_pair);
//    }
//    return result;
//}