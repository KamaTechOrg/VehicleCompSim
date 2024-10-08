#include "sensorrunner.h"
#include "CMakeUtils/../processControls.h"
#include "globalstate.h"

#define VERIFY_STATE(value) if (state != value) return onError()

const QString commandRunFormat =
#ifdef _WIN32
    "cmd /c \"%1\""
#else
    "bash -c '%1'",   // Bash (Unix-like)
#endif

    ;
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
    if (state = RUNNING) state = NOT_RUNNING;
}

void SensorRunner::onPrepareStep()
{
    VERIFY_STATE(NOT_RUNNING);

    if (!sensorModel->buildCommand().isEmpty())
    {
        process.startCommand(commandRunFormat.arg(sensorModel->buildCommand()));
        qInfo() << "prepare-pid: " << process.processId();
        QString processIdString = "prepare-pid: " + QString::number(process.processId());
        GlobalState::getInstance().log(processIdString, "Terminal");
        state = RUNNING;
    }
    else emit stepSuccess(currentStep);
}

void SensorRunner::onInitStep()
{
    VERIFY_STATE(NOT_RUNNING);

    if (sensorModel->runCommand().isEmpty()) return onError();

    process.startCommand(commandRunFormat.arg(sensorModel->runCommand()));
    qInfo() << "init-pid: " << process.processId();
    QString processIdString = "init-pid: " + QString::number(process.processId());
    GlobalState::getInstance().log(processIdString, "Terminal");
    //process.waitForStarted();
    //suspendProcess(process.processId());
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

    if (process.exitCode()) {
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
    GlobalState::getInstance().log(process.error() != QProcess::UnknownError ? unknownErrorAccureMessage : "",
                                   "Terminal");
    qInfo() << process.errorString();
    GlobalState::getInstance().log(process.errorString(), "Terminal");
}
