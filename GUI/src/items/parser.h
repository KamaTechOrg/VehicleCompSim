//
// Created by OWNER on 23/09/2024.
//

#ifndef MYPROJECT_PARSER_H
#define MYPROJECT_PARSER_H


#include <QVariant>
#include <QByteArray>

class parser :public QObject{
Q_OBJECT
public:
    parser();
public slots:
    void parseBuffer(const QString& data);
    void setSensorInfoMap(QMap<int, QList<QList<QString>>> parseInfoMap);
private:
    QMap<int, QList<QList<QString>>> sensorInfoMap;

};


#endif //MYPROJECT_PARSER_H
