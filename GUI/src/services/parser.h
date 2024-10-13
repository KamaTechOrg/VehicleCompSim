//
// Created by OWNER on 23/09/2024.
//

#ifndef MYPROJECT_PARSER_H
#define MYPROJECT_PARSER_H


#include <QVariant>
#include <QByteArray>
#include "constants.h"

class parser :public QObject{
Q_OBJECT
public:
    parser();
    QList<QPair<QString, QString>> extractBufferData(const QByteArray& buffer, size_t bufferSize, const QList<QList<QString>>& columnInfo);
public slots:
    void parseBuffer(const QString& buffer, size_t bufferSize);
    void setSensorInfoMap(QMap<int, QList<QList<QString>>> parseInfoMap);
private:
    QMap<int, QList<QList<QString>>> sensorInfoMap = {};
};


#endif //MYPROJECT_PARSER_H
