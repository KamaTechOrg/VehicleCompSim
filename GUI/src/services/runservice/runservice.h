#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include <QObject>
#include "runmanager.h"

class RunService : public QObject
{
    Q_OBJECT
    std::shared_ptr<RunManager> runManager;

public:
    RunService();

    void start();
    void stop();

signals:
    void startBegin();
    void stopFinished();

};

#endif // RUNSERVICE_H
