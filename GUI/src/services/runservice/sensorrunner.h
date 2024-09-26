#ifndef SENSORRUNNER_H
#define SENSORRUNNER_H

#include <QProcess>
#include <QObject>
#include "runner.h"
#include "models/sensormodel.h"

class SensorRunner : public Runner
{
    QProcess process;
    SensorModel* sensorModel;
public:
    SensorRunner(SensorModel* _model);

    // Runner interface
public slots:
    void step(RunSteps step);
    void abort();

private:
    void onPrepareStep();
    void onInitStep();
    void onStartStep();
    void onProcessFinished();
    void onError();

};

#endif // SENSORRUNNER_H
