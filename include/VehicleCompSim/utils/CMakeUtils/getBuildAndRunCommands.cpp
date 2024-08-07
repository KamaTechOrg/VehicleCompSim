#include "getBuildAndRunCommands.h"
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

std::pair<QString, QString> getBuildAndRunCommands(const QString &cmakePath) {
    QString projectPath = QDir(cmakePath.trimmed()).path();
    QString debugPath = QDir(cmakePath).filePath("Debug");
    QString relesePath = QDir(cmakePath).filePath("Release");
    //QDir().mkdir(buildPath); // Ensure the build directory exists

    QString projectName = extractProjectName(projectPath);
    if (projectName.isEmpty()) {
        qDebug() << "Failed to extract project name.";
        return std::make_pair(QString(), QString());
    }

    QStringList buildlist;
    buildlist <<
#ifdef Q_OS_WIN
        "cmd.exe" << "/c";
#else
        "sh" << "-c";
#endif

    buildlist << "cmake" << "-S" << projectPath << "-B" << projectPath;

    buildlist << ("-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=" + projectPath);
    buildlist << ("-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=" + debugPath);
    buildlist << ("-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=" + relesePath);

    buildlist << "&&";
    buildlist << "cmake" <<  "--build" << projectPath;

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
