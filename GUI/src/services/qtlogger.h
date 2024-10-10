#pragma once

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtMessageHandler>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

class QtLogger {
public:
    static void initialize();
    static void cleanup();

    static QFile logFile;
    static QTextStream logStream;

private:
    static QString generateLogFileName();
};
