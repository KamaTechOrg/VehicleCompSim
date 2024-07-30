#include "run.h"
#include <QDebug>




std::shared_ptr<QProcess> Run::parse_and_run_command(QString command)
{
    QStringList list = command.split(' ');
    for (auto& item: list) qInfo() << item << "--";
    QString program = list[0];
    list.pop_front();
    for (auto& item: list) qInfo() << item << "--";
    qInfo() << '\n';

    return runCommand(program, list);
}

std::shared_ptr<QProcess> Run::runCommand(const QString &program, const QStringList& arguments)
{
    std::shared_ptr<QProcess> process_ptr(new QProcess);
    QProcess& process = *process_ptr;

    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    process.setProgram(program);
    process.setArguments(arguments);

    QObject::connect(&process, &QProcess::errorOccurred, [&](QProcess::ProcessError error){
        qDebug() << "Error occurred: " << error << '\n';
        qDebug() << "Error string: " << process.errorString();
    });

    QObject::connect(&process, &QProcess::readyReadStandardOutput, [&](){
        qDebug() << process.readAllStandardOutput();
    });

    QObject::connect(&process, &QProcess::readyReadStandardError, [&](){
        qDebug() << process.readAllStandardError();
    });

    process.start();

    if (!process.waitForStarted()) {
        qDebug() << "Failed to start process:" << process.errorString();
    }
    // else if (!process.waitForFinished()) {
    //     qDebug() << "Failed to finish process:" << process.errorString();
    // }

    return process_ptr;
}










// Run::Run() {}

// Run::Run(std::vector<QString> _paths) : paths(_paths)
// {
// }

// void Run::run()
// {
//     for (auto&path: paths)
//     {
//         qInfo() << "Path: " << path << "output: " << compile_cmake(path) << "\n";
//     }
// }

// int Run::compile_cmake(QString& cmakeListsPath){
//     int commandOutput;
//     QString program = "cmake";
//     QStringList projectCreateArguments, projectBuildArguments;

//     projectCreateArguments << "-B" << cmakeListsPath << "-S" << cmakeListsPath;
//     projectBuildArguments << "--build" << cmakeListsPath;

//     commandOutput = runCommand(program, projectCreateArguments);
//     if (commandOutput != 0) return commandOutput;
//     return runCommand(program, projectBuildArguments);
// }

// int Run::runCommand(const QString &program, const QStringList& arguments)
// {
//     QProcess process;

//     process_ptr->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
//     process_ptr->setProgram(program);
//     process_ptr->setArguments(arguments);

//     QObject::connect(&process, &QProcess::errorOccurred, [&](QProcess::ProcessError error){
//         qDebug() << "Error occurred:" << error;
//     });

//     QObject::connect(&process, &QProcess::readyReadStandardOutput, [&](){
//         qDebug() << process_ptr->readAllStandardOutput();
//     });

//     QObject::connect(&process, &QProcess::readyReadStandardError, [&](){
//         qDebug() << process_ptr->readAllStandardError();
//     });

//     process_ptr->start();

//     if (!process_ptr->waitForStarted()) {
//         qDebug() << "Failed to start process:" << process_ptr->errorString();
//     }
//     else if (!process_ptr->waitForFinished()) {
//         qDebug() << "Failed to finish process:" << process_ptr->errorString();
//     }

//     return process_ptr->exitCode();
// }

