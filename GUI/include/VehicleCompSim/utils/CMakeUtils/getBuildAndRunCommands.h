#ifndef GETBUILDANDRUNCOMMANDS_H
#define GETBUILDANDRUNCOMMANDS_H

#include <QString>
#include <utility>

namespace CMakeUtils {

std::pair<QString, QString> getBuildAndRunCommands(const QString &sourcePath);

}

#endif // GETBUILDANDRUNCOMMANDS_H
