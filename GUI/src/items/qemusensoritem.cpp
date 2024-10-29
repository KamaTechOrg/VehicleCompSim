#include "qemusensoritem.h"
#include "models/qemusensormodel.h"

QemuSensorItem::QemuSensorItem(QemuSensorModel* _model) :SensorItem(_model), m_Qemu_model(_model)
{
}

QemuSensorModel &QemuSensorItem::getQemuModel()
{
    return *m_Qemu_model;
}




