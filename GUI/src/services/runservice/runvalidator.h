#ifndef RUNVALIDATOR_H
#define RUNVALIDATOR_H

#include "state/globalstate.h"

namespace RunValidator
{
QList<SerializableItem*> models();

bool doRunAllowd();
bool doAddSensor(SensorModel* sensor);

};

#endif // RUNVALIDATOR_H
