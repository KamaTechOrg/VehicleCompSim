#ifndef RUNCOMMAND_H
#define RUNCOMMAND_H

#include <QString>
#include <QProcess>

class RunCommand
{
public:

    // static function to initiate a process of the specified program with the given arguments
    static std::shared_ptr<QProcess> runCommand(const QString &program, const QStringList& arguments);

    // static helper function that initiate a process fom a single line of command
    // work by splitting the command into program's name (first segment in command) an arguments (rest of the segments)
    // then its uses Run::run_command to initiate the process
    static std::shared_ptr<QProcess> parse_and_run_command(QString command);
};

#endif // RUNCOMMAND_H
