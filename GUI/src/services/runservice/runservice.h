#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include <QObject>
#include "runmanager.h"

class RunService : public QObject
{
    Q_OBJECT
    std::shared_ptr<RunManager> runManager;
    static int newSessionId();
    int session;
    void startTimer(int timer);
public:
    RunService();

    void start(int timer);
    void stop();

signals:
    void startBegin();
    void stopFinished();

};

#endif // RUNSERVICE_H
