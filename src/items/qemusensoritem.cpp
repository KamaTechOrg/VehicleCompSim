#include "qemusensoritem.h"
#include "editors/QemuSensorItem_Editor.h"
#include "models/qemusensormodel.h"

QemuSensorItem::QemuSensorItem(QemuSensorModel* _model) :SensorItem(_model), m_Qemu_model(_model)
{
}

QemuSensorModel &QemuSensorItem::getQemuModel()
{
    return *m_Qemu_model;
}


void QemuSensorItem::openEditor()
{
    EditPanel::loadNewEditor(new QemuSensorItem::Editor(this));

}


