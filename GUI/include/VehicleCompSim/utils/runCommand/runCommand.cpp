#include "runCommand.h"
#include <QDebug>




std::shared_ptr<QProcess> RunCommand::parse_and_run_command(QString command)
{
    QStringList list = command.split(' ');
    QString program = list.takeFirst();

    return runCommand(program, list);
}

std::shared_ptr<QProcess> RunCommand::runCommand(const QString &program, const QStringList& arguments)
{
    std::shared_ptr<QProcess> process_ptr(new QProcess);
    std::weak_ptr<QProcess> weak_process_ptr(process_ptr); // waek_ptr is used to safly calling the qprocess withoout incrementing its ptr count

    process_ptr->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    process_ptr->setProgram(program);
    process_ptr->setArguments(arguments);

    QObject::connect(process_ptr.get(), &QProcess::errorOccurred, [weak_process_ptr](QProcess::ProcessError error){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug() << "Error occurred: " << error << '\n';
            qDebug() << "Error string: " << process_ptr->errorString();
        }
    });

    QObject::connect(process_ptr.get(), &QProcess::readyReadStandardOutput, [weak_process_ptr](){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug() << process_ptr->readAllStandardOutput();
        }
    });

    QObject::connect(process_ptr.get(), &QProcess::readyReadStandardError, [weak_process_ptr](){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug() << process_ptr->readAllStandardError();
        }
    });

    process_ptr->start();

    if (!process_ptr->waitForStarted()) {
        qDebug() << "Failed to start process:" << process_ptr->errorString();
    }

    return process_ptr;
}

