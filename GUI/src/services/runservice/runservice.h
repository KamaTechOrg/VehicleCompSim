#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include <QObject>
#include "manger.h"
#include "runmanager.h"
//#include "../../../Communication/server/include/manger.h"
#include "../../../../Communication/server/include/manger.h"




class RunService : public QObject
{
    Q_OBJECT
    std::shared_ptr<RunManager> runManager;
    static int newSessionId();
    int session;
    MangServer server;

    void startTimer(int timer);
    void initComServer(QString com_server_ip);
    void closeComServer();
public:
    RunService();

    void start(int timer, QString &com_server_ip);
    void stop();
signals:
    void startBegin();
    void stopFinished();

};

#endif // RUNSERVICE_H
