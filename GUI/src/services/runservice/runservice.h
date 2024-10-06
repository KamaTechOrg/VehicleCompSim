#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include <QObject>
#include "manger.h"
#include "runmanager.h"
//#include "../../../Communication/server/include/manger.h"
#include "../../../../Communication/server/include/manger.h"
#include "../../../../Communication/client/include/client.h"




class RunService : public QObject
{
    Q_OBJECT
    std::shared_ptr<RunManager> runManager;
    static int newSessionId();
    int session;

    std::shared_ptr<MangServer> server;
    std::shared_ptr<std::thread> server_thread;

    std::shared_ptr<ClientSocket> client;
    std::shared_ptr<std::thread> client_listner;

    void startTimer(int timer);
    void initComServer(QString com_server_ip);
    void closeComServer();

    void startAlphaClient();
    void closeAlphaClient();

public:
    RunService();

    void start(int timer, QString &com_server_ip);
    void stop();
signals:
    void startBegin();
    void stopFinished();
    void newCommunicationPacketAccepted(QString packet);

};

#endif // RUNSERVICE_H
