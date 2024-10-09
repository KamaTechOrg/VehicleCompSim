#include "Logger.h"
#include <QDir>

QFile Logger::logFile;
QTextStream Logger::logStream;

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context);

    QString logMessage;
    switch (type) {
        case QtInfoMsg:
            logMessage = QString("Info: %1").arg(msg);
            break;
        case QtDebugMsg:
            logMessage = QString("Debug: %1").arg(msg);
            break;
        case QtWarningMsg:
            logMessage = QString("Warning: %1").arg(msg);
            break;
        case QtCriticalMsg:
            logMessage = QString("Critical: %1").arg(msg);
            break;
        case QtFatalMsg:
            logMessage = QString("Fatal: %1").arg(msg);
            break;
    }

    Logger::logStream << QDateTime::currentDateTime().toString(Qt::ISODate) << " - " << logMessage << Qt::endl;
    Logger::logStream.flush();

    if (type == QtFatalMsg) {
        abort();
    }
}

QString Logger::generateLogFileName() {
    QDir logDir("logs");
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    return logDir.filePath(QString("log_%1.txt").arg(timestamp));
}

void Logger::initialize() {
    logFile.setFileName(generateLogFileName());
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logStream.setDevice(&logFile);
        qInstallMessageHandler(customMessageHandler);
    }
}

void Logger::cleanup() {
    qInstallMessageHandler(nullptr);
    logFile.close();
}