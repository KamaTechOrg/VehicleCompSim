#include "runservice.h"
#include "runservice/runvalidator.h"
#include "state/globalstate.h"
#include "sensorrunner.h"
#include "IpFileHanler.h"
#include <thread>

int RunService::newSessionId()
{
    static int id = 0;
    return ++id;
}

void RunService::startTimer(int timer)
{
    if (timer < 1) return;

    auto _session = session;
    std::thread([this, timer, _session](){
        std::this_thread::sleep_for(std::chrono::seconds(timer));
        if (_session == session) this->stop();
    }).detach();
}

RunService::RunService() :server (new MangServer)
{
    clearIpFile();

    server_thread.reset(new std::thread([this](){
        server->init();
    }));
}

void RunService::start(int timer, QString& com_server_ip)
{
    // write the coms server's ip
    session = newSessionId();
    runManager = std::make_shared<RunManager>();
    QObject::connect(runManager.get(), &RunManager::startBegin, [this, timer](){
        startAlphaClient();
        emit startBegin();
        startTimer(timer);
    });
    QObject::connect(runManager.get(), &RunManager::stopFinished, [this](){emit stopFinished();});

    for (auto item: GlobalState::getInstance().currentProject()->models())
    {
        if (auto model = dynamic_cast<SensorModel*>(item))
        {
            if (RunValidator::doAddSensor(model))
            {
                runManager->addRunner(std::make_shared<SensorRunner>(model));
            }
        }    }
    runManager->start();
}

void RunService::stop()
{
    if (!runManager) return;
    runManager->stop();
    closeComServer();
}

void RunService::initComServer(QString com_server_ip)
{
    if (com_server_ip.isEmpty())
    {
        // should load the     comunication server here
    }
    else
    {
        writeIpFile(com_server_ip.toStdString());
    }
}

void RunService::closeComServer()
{
    clearIpFile();
}

void RunService::startAlphaClient()
{
    static bool init = true;
    if (init)
    {
        client.reset(new ClientSocket(100));

        client_listner.reset(new std::thread([this](){
            char buffer[MAXRECV];

            while (true)
            {
                memset(buffer, 0, sizeof(buffer));
                auto pair_recv = client->listen(buffer, sizeof(buffer));
                if (pair_recv.first != ListenErrorCode::SUCCESS)
                {
                    client->shut_down();
                    return 1;
                }
                emit newCommunicationPacketAccepted(QString(buffer));
            }
        }));
        init = false;
    }
}

