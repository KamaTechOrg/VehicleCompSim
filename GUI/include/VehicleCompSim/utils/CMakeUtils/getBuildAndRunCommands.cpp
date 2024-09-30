#include "getBuildAndRunCommands.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

#define QUOTE_STR  QString("\"")

namespace CMakeUtils {



QString extractProjectName(const QString &sourcePath) {
    QFile cmakeFile(QDir(sourcePath).filePath("CMakeLists.txt"));
    if (!cmakeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open CMakeLists.txt";
        return QString();
    }

    QTextStream in(&cmakeFile);
    QRegularExpression re("^\\s*project\\s*\\(([^\\s\\)]+)\\s*(.*)\\)\\s*$", QRegularExpression::CaseInsensitiveOption);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            return match.captured(1);
        }
    }

    qDebug() << "Project name not found in CMakeLists.txt";
    return QString();
}

std::pair<QString, QString> getBuildAndRunCommands(const QString &cmakePath) {
    QString projectPath = QFileInfo(cmakePath.trimmed()).absolutePath();
    QString buildPath = QDir(projectPath).filePath("build");
    QString debugPath = QDir(buildPath).filePath("Debug");
    QString relesePath = QDir(buildPath).filePath("Release");
    //QDir().mkdir(buildPath); // Ensure the build directory exists

    QString projectName = extractProjectName(projectPath);
    if (projectName.isEmpty()) {
        qDebug() << "Failed to extract project name.";
        return std::make_pair(QString(), QString());
    }

    QStringList buildlist;


    buildlist << "cmake" << "-S" <<  QUOTE_STR + projectPath + QUOTE_STR << "-B" << QUOTE_STR + buildPath + QUOTE_STR;

    buildlist << (QString("-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=") + QUOTE_STR +  buildPath + QUOTE_STR);
    buildlist << (QString("-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=") + QUOTE_STR + debugPath +  QUOTE_STR);
    buildlist << (QString("-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=") +  QUOTE_STR + relesePath +  QUOTE_STR);

    buildlist << "&&";
    buildlist << "cmake" <<  "--build" << QUOTE_STR + buildPath + QUOTE_STR;

    QString buildCommand = buildlist.join(' ');
    QString runCommand = QDir(debugPath).filePath(projectName);
#ifdef Q_OS_WIN
    runCommand += ".exe";
#endif

    qInfo() << buildCommand;
    qInfo() << runCommand;
    return std::make_pair(buildCommand, runCommand);
}

}
