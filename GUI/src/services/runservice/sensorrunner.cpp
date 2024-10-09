#include "sensorrunner.h"
#include <QFile>

#define VERIFY_STATE(value) if (state != value) return onError()

const QString commandRunFormat =
#ifdef _WIN32
    R"(cmd /c "%1")"
#else
    R"(bash -c '%1')",   // Bash (Unix-like)
#endif
    ;
void log(QString l)
{

    // Get the current date and time with milliseconds
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_z");

    // Create a file name using the timestamp
    QString filePath = QString("C:/codes/output_%1.txt").arg(timestamp);

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << l;
        file.close();
    } else {
        // Handle error
    }

}
SensorRunner::SensorRunner(SensorModel *_model) :
    sensorModel(_model)
{
    QObject::connect(&process, &QProcess::finished, [this](){
        VERIFY_STATE(RUNNING);
        onProcessFinished();
    });
    QObject::connect(&process, &QProcess::errorOccurred, [this](){
        onError();
    });
    QObject::connect(&process, &QProcess::readyReadStandardOutput, [this](){
        qDebug() << process.readAllStandardOutput();
    });
}

void SensorRunner::step(RunSteps step)
{
    if (step != currentStep + 1) return onError();

    currentStep = step;

    switch (step) {
    case PREPARE:
        onPrepareStep();
        break;
    case INIT:
        onInitStep();
        break;
    case START:
        onStartStep();
        break;
    default:
        break;
    }
}

void SensorRunner::abort()
{
    if (process.state() == QProcess::Starting ||
        process.state() == QProcess::Running)
    {
        // process.terminate();
        // if (!process.waitForFinished(3000)) { // wait for 3 seconds
        //     process.close();
        // }
        // if (process.exitCode())
        // {
        //     onError();
        // }
        process.kill(); // not good option
    }
    if (state == RUNNING) state = NOT_RUNNING;
}

void SensorRunner::onPrepareStep()
{
    VERIFY_STATE(NOT_RUNNING);

    if (!sensorModel->buildCommand().isEmpty())
    {
        QString command = commandRunFormat.arg(sensorModel->buildCommand());
        log(command);
        //process.startCommand( command);
        process.setProgram("cmd");
        process.setArguments({"/c", sensorModel->buildCommand()});
        process.start();
        state = RUNNING;
    }
    else emit stepSuccess(currentStep);
}

void SensorRunner::onInitStep()
{
    VERIFY_STATE(NOT_RUNNING);

    if (sensorModel->runCommand().isEmpty()) return onError();

    QString command = commandRunFormat.arg(sensorModel->runCommand());
    log(command);
    process.startCommand( command);
                                                                        //process.waitForStarted();
                                  // CODE FOR SIMULTAINUS RUNNING       //suspendProcess(process.processId());
                                                                        //state = SUSPENDED;
    state = RUNNING;
    emit Runner::stepSuccess(currentStep);
}

void SensorRunner::onStartStep()
{
    //VERIFY_STATE(SUSPENDED);

    //resumeProcess(process.processId());
    //state = RUNNING;
    emit Runner::stepSuccess(currentStep);
}

void SensorRunner::onProcessFinished()
{
    VERIFY_STATE(RUNNING);
    qInfo() << "process.exitCode(): " << process.exitCode();
    if (false && process.exitCode()) {
        onError();
    }
    else {
        state = NOT_RUNNING;
        emit Runner::stepSuccess(currentStep);
    }
}

void SensorRunner::onError()
{
    QString unknownErrorAccureMessage = "some running error accure with sensorRunner: " + sensorModel->name();

    emit Runner::errorAccure();
    state = HAS_ERROR;
    qInfo() << (process.error() != QProcess::UnknownError ? unknownErrorAccureMessage : "");
    qInfo() << process.errorString();
}
