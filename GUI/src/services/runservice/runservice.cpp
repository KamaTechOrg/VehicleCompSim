#include "runservice.h"
#include "state/globalstate.h"
#include "sensorrunner.h"
#include "IpFileHanler.h"

RunService::RunService()
{
    clearIpFile();
}



void RunService::start()
{
    // write the coms server's ip

    runManager = std::make_shared<RunManager>();
    for (auto item: GlobalState::getInstance().currentProject()->models())
    {
        if (auto model = dynamic_cast<SensorModel*>(item))
            runManager->addRunner(std::make_shared<SensorRunner>(model));
    }
    QObject::connect(runManager.get(), &RunManager::startBegin, [this](){emit startBegin();});
    QObject::connect(runManager.get(), &RunManager::stopFinished, [this](){emit stopFinished();});
    runManager->start();
}

void RunService::stop()
{
    if (!runManager) return;

    runManager->stop();
}
