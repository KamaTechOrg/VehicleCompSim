#include "getBuildAndRunCommands.h".h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

namespace CMakeUtils {

QString getExecutablePath(const QString &buildPath, const QString &projectName) {
    QString executablePath = QDir(buildPath).filePath(projectName);
#ifdef Q_OS_WIN
    executablePath += ".exe";
#endif
    return executablePath;
}

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

std::pair<QString, QString> getBuildAndRunCommands(const QString &sourcePath) {
    QString buildPath = QDir(sourcePath).filePath("Debug");
    //QDir().mkdir(buildPath); // Ensure the build directory exists

    QString projectName = extractProjectName(sourcePath);
    if (projectName.isEmpty()) {
        qDebug() << "Failed to extract project name.";
        return std::make_pair(QString(), QString());
    }

    QString buildCommand = QString("cmake --build %1").arg(sourcePath);
    QString executablePath = getExecutablePath(buildPath, projectName);
    QString runCommand = QString("%1").arg(executablePath);

    return std::make_pair(buildCommand, runCommand);
}

}
