#include "runvalidator.h"



bool RunValidator::doRunAllowd()
{
    for (auto model: models())
    {
        if (auto sensor = dynamic_cast<SensorModel*>(model))
        {
            if (sensor->runCommand().isEmpty()) return false;
        }
    }
    return true;
}

bool RunValidator::doAddSensor(SensorModel* sensor)
{

    if (sensor->ownerID() != GlobalState::getInstance().myClientId()) return false;
    if (sensor->isExcludeFromProject()) return false;

    return true;
}

QList<SerializableItem *> RunValidator::models()
{
    return GlobalState::getInstance().currentProject()->models();
}
