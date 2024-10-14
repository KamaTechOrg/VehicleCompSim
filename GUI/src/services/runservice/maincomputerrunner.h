#ifndef MAINCOMPUTERRUNNER_H
#define MAINCOMPUTERRUNNER_H
#include <QProcess>
#include <QObject>
#include "models/maincomputermodel.h"
#include "runner.h"

class MainComputerRunner : public Runner
{
    QProcess process;
    MainComputerModel* mcModel;
public:
    MainComputerRunner( MainComputerModel* _mcModel);

    // Runner interface
public slots:
    void step(RunSteps step);
    void abort();

private:
    void onStartStep();
};

#endif // MAINCOMPUTERRUNNER_H
