#ifndef RUNNER_H
#define RUNNER_H

#include <QObject>

class Runner : public QObject
{
    Q_OBJECT
public:
    enum RunnerState {
        NOT_RUNNING,
        SUSPENDED,
        RUNNING,
        HAS_ERROR
    };
    enum RunSteps {
        ZERO,
        PREPARE,
        INIT,
        START,
        RUN_STEPS_COUNT
    };

    RunnerState state = NOT_RUNNING;
protected:
    RunSteps currentStep = ZERO;

signals:
    void stepSuccess(RunSteps step);
    void errorAccure();

public slots:
    virtual void step(RunSteps step) = 0;
    virtual void abort() = 0;
};

#endif // RUNNER_H
