#include "runservice.h"
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

RunService::RunService()
{
    clearIpFile();
}



void RunService::start(int timer)
{
    // write the coms server's ip
    session = newSessionId();
    runManager = std::make_shared<RunManager>();
    QObject::connect(runManager.get(), &RunManager::startBegin, [this, timer](){
        emit startBegin();
        startTimer(timer);
    });
    QObject::connect(runManager.get(), &RunManager::stopFinished, [this](){emit stopFinished();});

    for (auto item: GlobalState::getInstance().currentProject()->models())
    {
        if (auto model = dynamic_cast<SensorModel*>(item))
            runManager->addRunner(std::make_shared<SensorRunner>(model));
    }
    runManager->start();
}

void RunService::stop()
{
    if (!runManager) return;

    runManager->stop();
}

