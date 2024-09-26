#ifndef RUNMANAGER_H
#define RUNMANAGER_H

#include <QObject>
#include "runner.h"

class RunManager : public QObject
{
    Q_OBJECT

    std::vector<std::shared_ptr<Runner>> runners;
    Runner::RunSteps currentStep = Runner::ZERO;

    int countRunnersFinishedStep = 0;

    void nextStep();
public:
    RunManager();
    ~RunManager();

    void addRunner(std::shared_ptr<Runner> _runner);

    void start();
    void stop();

signals:
    void startBegin();
    void stopFinished();

    void step(Runner::RunSteps step);
    void abort();

private slots:
    void onRunnerFinishedStep(Runner::RunSteps step);
    void onRunnerAccureError();
};

#endif // RUNMANAGER_H
