#include "maincomputerrunner.h"

MainComputerRunner::MainComputerRunner(MainComputerModel* _mcModel):
    mcModel(_mcModel)
{

}

void MainComputerRunner::step(RunSteps step)
{
    if (step != currentStep + 1)
    {
        qDebug() << "run step mismatch at main computer";
        emit Runner::errorAccure();
        return;
    }

    currentStep = step;

    switch (step) {
    case START:
        onStartStep();
        break;
    default:
        emit stepSuccess(step);
        break;
    }
}

void MainComputerRunner::abort()
{
    if (currentStep == START)
    {
        mcModel->computer().stop();
        state = NOT_RUNNING;
    }
}

void MainComputerRunner::onStartStep()
{
    mcModel->computer().run();
    state = RUNNING;
    emit Runner::stepSuccess(currentStep);
}
